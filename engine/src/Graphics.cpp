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

uint32_t VertexLayout::hash(const VertexElement* elements, size_t count)
{
	return 0;
}

static bool InputElementCmp(const VertexElement& e1, const VertexElement& e2)
{
	if (e1.slot < e2.slot)
		return true;
	if (e1.semantic < e2.semantic)
		return true;
	return false;
}

VertexLayout::VertexLayout(const VertexElement* elements, size_t count)
{
	static const PixelFormat g_types[SEMANTIC_MAX] = 
	{
		PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3,
		PF_FLOAT4, PF_UINT4,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2
	};

	typedef std::vector<int> StrideVec;

	// 排序
	bool needSort = false;
	m_elements.resize(count);

	StrideVec strides(count);

	for (size_t i = 0; i < count; ++i)
	{
		VertexElement& elem = m_elements[i];
		elem = elements[i];

		if (elem.format == PF_UNKNOWN)
			elem.format = g_types[(int)elem.semantic];

		if (elem.rate == INPUT_RATE_INSTANCE)
			m_instanced = true;

		if (!needSort && i > 0)
		{
			VertexElement& prev = m_elements[i - 1];
			if (elem.slot < prev.slot)
				needSort = true;
			else if (elem.semantic < prev.semantic)
				needSort = true;
		}

		strides[elem.slot] += PixelUtil::getBytes(elem.format);
	}

	// 设置跨度
	for (size_t i = 0; i < count; ++i)
	{
		VertexElement& elem = m_elements[i];
		elem.stride = strides[elem.slot];
	}

	if (needSort)
	{
		std::sort(m_elements.begin(), m_elements.end(), &InputElementCmp);
	}

	// 计算偏移
	VertexElement* prev;
	VertexElement* curr;
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

	m_hash = hash(elements, count);
}

bool VertexLayout::equal(const VertexElement* elements, size_t count) const
{
	if (m_elements.size() != count)
		return false;
	for (size_t i = 0; i < count; ++i)
	{
		const VertexElement& e1 = m_elements[i];
		const VertexElement& e2 = elements[i];
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

VertexArray::VertexArray(VertexLayout* layout)
	: m_layout(layout)
	, m_dirty(true)
	, m_startSlot(0)
{

}

void VertexArray::setLayout(VertexLayout* layout)
{
	if (m_layout != layout)
	{
		m_layout = layout;
		m_dirty = true;
	}
}

void VertexArray::setBuffer(GpuBuffer* buffer, size_t slot /* = 0 */, size_t offset)
{
	if (slot >= m_buffers.size())
	{
		m_buffers.resize(slot + 1);
		m_offsets.resize(slot + 1);
	}

	m_buffers[slot] = buffer;
	m_offsets[slot] = offset;
	m_dirty = true;
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

VertexLayout* Device::newVertexLayout(const VertexElement* elements, size_t count)
{
	return new VertexLayout(elements, count);
}

VertexArray* Device::newVertexArray(VertexLayout* layout)
{
	return new VertexArray(layout);
}

GpuBuffer* Device::newVertexBuffer(uint32_t stride, uint32_t counts, const void* data /* = NULL */, RES_FLAG flags /* = RES_DEFAULT */)
{
	BufferDesc desc(BU_VERTEX, stride, counts, flags, data);
	return newBuffer(desc);
}

GpuBuffer* Device::newIndexBuffer(IndexType type, uint32_t counts, const void* data /* = NULL */, RES_FLAG flags /* = RES_DEFAULT */)
{
	uint32_t stride = type == INDEX16 ? sizeof(uint16_t) : sizeof(uint32_t);
	BufferDesc desc(BU_INDEX, stride, counts, flags, data);
	return newBuffer(desc);
}

GpuBuffer* Device::newUniformBuffer(uint32_t bytes, const void* data /* = NULL */, RES_FLAG flags /* = RES_DYNAMIC */)
{
	BufferDesc desc(BU_UNIFORM, bytes, 1, flags, data);
	return newBuffer(desc);
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

CU_NS_END