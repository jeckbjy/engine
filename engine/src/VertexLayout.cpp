#include "VertexLayout.h"
#include "Graphics.h"
#include "Util.h"

CU_NS_BEGIN

VertexDeclaration::VertexDeclaration()
:m_builded(FALSE), m_code(0), m_stride(0)
{

}

VertexDeclaration::VertexDeclaration(const VertexElement* elements, size_t num)
	: m_builded(FALSE), m_code(0), m_stride(0)
{
	m_elements.reserve(num);
	for (size_t i = 0; i < num; ++i)
	{
		m_elements.push_back(elements[i]);
	}
	// 计算
	build();
}

void VertexDeclaration::build()
{
	if (m_builded == TRUE)
		return;
	m_builded = TRUE;
	static const PixelFormat g_types[SEMANTIC_MAX] =
	{
		PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3, PF_FLOAT3,
		PF_FLOAT4, PF_UINT4,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2,
		PF_FLOAT2, PF_FLOAT2, PF_FLOAT2, PF_FLOAT2
	};
	m_flag = 0;
	// 计算跨度
	size_t num = size();
	std::vector<int> strides(num);
	for (size_t i = 0; i < num; ++i)
	{
		VertexElement& ve = m_elements[i];
		if (ve.type == PF_UNKNOWN)
			ve.type = g_types[ve.usage];
		ve.offset = strides[ve.slot];
		m_flag |= (1 << ve.usage);
		strides[ve.slot] += PixelUtil::getBytes(ve.type);
	}
	// 设置跨度
	for (size_t i = 0; i < num; ++i)
	{
		VertexElement& ve = m_elements[i];
		ve.stride = strides[ve.slot];
	}
	// 计算hash
	m_code = Util::hash_bkdr((const char*)&at(0), sizeof(VertexElement) * size());
	m_stride = strides.empty() ? 0 : strides[0];
}

bool VertexDeclaration::equal(const VertexDeclaration& other) const
{
	if (this->m_code != other.m_code)
		return false;
	if (this->size() != other.size())
		return false;
	for (size_t i = 0; i < other.size(); ++i)
	{
		if (this->at(i) != other.at(i))
			return false;
	}
	return true;
}

void VertexDeclaration::push_back(const VertexElement& ve)
{
	m_builded = FALSE;
	m_elements.push_back(ve);
}

void VertexDeclaration::push_back(Semantic usage, PixelFormat format /* = PF_UNKNOWN */, uint8_t slot)
{
	VertexElement ve;
	ve.usage = usage;
	ve.type = format;
	ve.slot = slot;
	push_back(ve);
}

int VertexDeclaration::getOffset(Semantic usage) const
{
	for (const_iterator itor = m_elements.begin(); itor != m_elements.end(); ++itor)
	{
		if (itor->usage == usage)
			return itor->offset;
	}
	return -1;
}

bool VertexDeclaration::operator==(const VertexDeclaration& rhs) const
{
	return m_code == rhs.m_code && m_elements == rhs.m_elements;
}

//////////////////////////////////////////////////////////////////////////
// VertexLayout
//////////////////////////////////////////////////////////////////////////
VertexLayout::VertexLayout(VertexDeclaration& desc)
	: m_dirty(true)
{
	desc.build();
}

void VertexLayout::setBuffer(VertexBuffer* vb, int slot /* = 0 */)
{
	m_buffers[slot] = vb;
	m_dirty = true;
}

size_t VertexLayout::getVertexCount() const
{
	if (m_buffers.empty())
		return 0;
	VertexBuffer* buffer = m_buffers.begin()->second;
	return buffer->count();
}

CU_NS_END
