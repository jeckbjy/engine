#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class CU_VK_API VK_VertexLayout : public VertexLayout
{
public:
	VK_VertexLayout(uint32_t id, const VertexElement* elements, size_t count);
	~VK_VertexLayout();

	VkPipelineVertexInputStateCreateInfo* getInfo() { return &m_info; }

private:
	VkPipelineVertexInputStateCreateInfo m_info;
	VkVertexInputBindingDescription*	m_bindings;
	VkVertexInputAttributeDescription*	m_attrs;
	size_t m_bindingCount;
	size_t m_attrCount;
};

CUTE_NS_END
