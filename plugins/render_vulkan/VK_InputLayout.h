#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_InputLayout : public InputLayout
{
public:
	VK_InputLayout(const InputElement* elements, size_t count);
	~VK_InputLayout();

	VkPipelineVertexInputStateCreateInfo* getInfo() { return &m_info; }

private:
	VkPipelineVertexInputStateCreateInfo m_info;
	VkVertexInputBindingDescription*	m_bindings;
	VkVertexInputAttributeDescription*	m_attrs;
	size_t m_bindingCount;
	size_t m_attrCount;
};

CU_NS_END
