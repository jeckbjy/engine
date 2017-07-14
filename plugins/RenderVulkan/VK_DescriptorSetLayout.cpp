#include "VK_VertexLayout.h"
#include "VK_Mapping.h"

CUTE_NS_BEGIN

VK_DescriptorSetLayout::VK_DescriptorSetLayout()
{
}

VK_DescriptorSetLayout::~VK_DescriptorSetLayout()
{
	term();
}

bool VK_DescriptorSetLayout::init(VK_Device* device)
{
	return true;
}

void VK_DescriptorSetLayout::term()
{
}


//struct BindInfo
//{
//	uint32_t stride;
//	InputRate rate;
//	BindInfo() :stride(0), rate(INPUT_RATE_VERTEX){}
//};
//
//VK_VertexLayout::VK_VertexLayout(uint32_t id, const VertexElement* elements, size_t count)
//	: VertexLayout(id, elements, count)
//	, m_bindings(NULL)
//	, m_attrs(NULL)
//{
//	m_attrCount = count;
//	m_attrs = new VkVertexInputAttributeDescription[m_attrCount];
//
//	// 统计binding,并fill Attribute
//	typedef std::map<uint32_t, BindInfo> BindingMap;
//	BindingMap bindings;
//	for (size_t i = 0; i < count; ++i)
//	{
//		const VertexElement& elem = m_elements[i];
//		BindInfo info = bindings[elem.slot];
//		info.stride += PixelUtil::getBytes(elem.format);
//		info.rate = elem.rate;
//	}
//
//	assert(bindings.size() > 0);
//	m_bindingCount = bindings.size();
//	m_bindings = new VkVertexInputBindingDescription[m_bindingCount];
//
//	size_t index = 0;
//	for (BindingMap::iterator itor = bindings.begin(); itor != bindings.end(); ++itor, ++index)
//	{
//		VkVertexInputBindingDescription& binding = m_bindings[index];
//		binding.binding = itor->first;
//		binding.stride = itor->second.stride;
//		binding.inputRate = (VkVertexInputRate)itor->second.rate;
//	}
//
//	// 初始化attribute
//	for (size_t i = 0; i < count; ++i)
//	{
//		VkVertexInputAttributeDescription& attr = m_attrs[i];
//		const VertexElement& elem = m_elements[i];
//		attr.location = 0;	// 需要通过prog才能确定??
//		attr.binding = elem.slot;
//		attr.format = VK_Mapping::getFormat(elem.format);
//		attr.offset = elem.offset;
//	}
//}
//
//VK_VertexLayout::~VK_VertexLayout()
//{
//	if (m_attrs)
//	{
//		delete[] m_attrs;
//	}
//
//	if (m_bindings)
//	{
//		delete[] m_bindings;
//	}
//}

CUTE_NS_END
