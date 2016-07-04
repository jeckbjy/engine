#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_VertexLayout : public VertexLayout
{
public:
	VK_VertexLayout(const VertexElement* elements, size_t count);
	~VK_VertexLayout();

	VkPipelineVertexInputStateCreateInfo* getInfo() { return &m_info; }

private:
	VkPipelineVertexInputStateCreateInfo m_info;
	VkVertexInputBindingDescription*	m_bindings;
	VkVertexInputAttributeDescription*	m_attrs;
	size_t m_bindingCount;
	size_t m_attrCount;
};

CU_NS_END
