#include "Graphics.h"

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

uint32_t InputLayout::hash(const InputElement* elements, size_t count)
{
	return 0;
}

InputLayout::InputLayout(const InputElement* elements, size_t count)
{
	static const PixelFormat g_types[SEMANTIC_MAX] = {
		PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3,
		PF_FLOAT4, PF_UINT4,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2
	};

	m_hash = hash(elements, count);

	// todo:自动统计offset
	m_elements.resize(count);
	for (size_t i = 0; i < count; ++i)
	{
		InputElement& elem = m_elements[i];
		elem = elements[i];
		if (elem.format == PF_UNKNOWN)
			elem.format = g_types[(int)elem.semantic];
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

CU_NS_END
