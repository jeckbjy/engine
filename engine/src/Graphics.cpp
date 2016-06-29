#include "Graphics.h"
#include <stdio.h>

CU_NS_BEGIN

Graphics gGraphics;

GpuBuffer::GpuBuffer(const BufferDesc& desc)
: m_flags(desc.flags)
, m_bytes(desc.counts * desc.stride)
, m_count(desc.counts)
, m_stride(desc.stride)
{
}

GpuBuffer::~GpuBuffer()
{

}

void GpuBuffer::write(const void* data, size_t len, size_t offset /* = 0 */)
{
	if (offset >= m_bytes)
		return;

	size_t end = offset + len;
	if (end > m_bytes)
		len = m_bytes - offset;
	char* ptr = (char*)map(offset, len, MAP_WRITE_ONLY);
	memcpy(ptr, data, len);
	unmap();
}

void GpuBuffer::read(void* data, size_t len, size_t offset /* = 0 */)
{
	if (offset >= m_bytes)
		return;

	size_t end = offset + len;
	if (end > m_bytes)
		len = m_bytes - offset;

	char* ptr = (char*)map(offset, len, MAP_READ_ONLY);
	memcpy(data, ptr, len);
	unmap();
}

uint32_t InputLayout::hash(const InputElement* elements, size_t count)
{
	return 0;
}

static bool InputElementCmp(const InputElement& e1, const InputElement& e2)
{
	if (e1.slot < e2.slot)
		return true;
	if (e1.semantic < e2.semantic)
		return true;
	return false;
}

InputLayout::InputLayout(const InputElement* elements, size_t count)
{
	static const PixelFormat g_types[SEMANTIC_MAX] = 
	{
		PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3,
		PF_FLOAT4, PF_UINT4,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2
	};

	m_hash = hash(elements, count);

	// 排序
	bool needSort = false;
	m_elements.resize(count);
	for (size_t i = 0; i < count; ++i)
	{
		InputElement& elem = m_elements[i];
		elem = elements[i];

		if (elem.format == PF_UNKNOWN)
			elem.format = g_types[(int)elem.semantic];

		if (elem.rate == INPUT_RATE_INSTANCE)
			m_instanced = true;

		if (!needSort && i > 0)
		{
			InputElement& prev = m_elements[i - 1];
			if (elem.slot < prev.slot)
				needSort = true;
			else if (elem.semantic < prev.semantic)
				needSort = true;
		}
	}

	if (needSort)
	{
		std::sort(m_elements.begin(), m_elements.end(), &InputElementCmp);
	}

	// 计算偏移
	InputElement* prev;
	InputElement* curr;
	for (size_t i = 1; i < count; ++i)
	{
		prev = &m_elements[i - 1];
		curr = &m_elements[i];

		if (prev->slot == curr->slot)
		{
			curr->offset = prev->offset + PixelUtil::getBytes(prev->format);
		}
		else
		{
			curr->offset = 0;
		}
	}
}

bool InputLayout::equal(const InputElement* elements, size_t count) const
{
	if (m_elements.size() != count)
		return false;
	for (size_t i = 0; i < count; ++i)
	{
		const InputElement& e1 = m_elements[i];
		const InputElement& e2 = elements[i];
		if (
			(e1.semantic != e2.semantic) ||
			(e1.format != e2.format) ||
			(e1.slot != e2.slot) ||
			(e1.offset != e2.offset) ||
			(e1.rate != e2.rate))
			return false;
	}

	return true;
}

void FrameBuffer::attach(size_t att, Texture* attachment)
{
	if (att >= ATTACHMENT_MAX)
		return;

	if (att >= m_attachments.size())
		m_attachments.resize(att + 1);


	m_attachments[att] = attachment;
	m_dirty = true;
}

void FrameBuffer::detach(size_t att)
{
	if (att < m_attachments.size())
	{
		m_attachments[att] = NULL;
		m_dirty = true;
	}
}

ShaderStage* Device::loadShader(ShaderType type, const String& path)
{
	FILE* fp = fopen(path.c_str(), "rt");
	if (fp == NULL)
		return NULL;
	size_t count = 0;
	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// 加载文件
	String code;
	code.resize(count);
	fread(&code[0], 1, count, fp);
	fclose(fp);

	ShaderDesc desc;
	desc.stage = type;
	desc.code = code;
	ShaderStage* shader = newShader();
	shader->compile(desc);
	return shader;
}

void CommandBuffer::setVertexBuffer(GpuBuffer* buffer, size_t offset /* = 0 */, size_t startSlot /* = 0 */)
{
	GpuBuffer* buffers[1] = { buffer };
	size_t offsets[1] = { offset };
	setVertexBuffers(startSlot, 1, buffers, offsets);
}

CU_NS_END