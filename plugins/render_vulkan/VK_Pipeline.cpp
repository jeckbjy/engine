#include "VK_Pipeline.h"

CU_NS_BEGIN

VK_Pipeline::VK_Pipeline(VK_Device* device, const GRAPHICS_PIPELINE_DESC& desc)
	: m_device(device)
	, m_handle(VK_NULL_HANDLE)
{
	VkPipelineShaderStageCreateInfo stages[5];

	VkGraphicsPipelineCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.pNext = NULL;
	// convert

	VK_CHECK(vkCreateGraphicsPipelines(*m_device, NULL, 1, &info, NULL, &m_handle), "vkCreateGraphicsPipelines fail!");
}

VK_Pipeline::VK_Pipeline(VK_Device* device, const COMPUTE_PIPELINE_DESC& desc)
	: m_device(device)
	, m_handle(VK_NULL_HANDLE)
{
	VkComputePipelineCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	info.pNext = NULL;

	VK_CHECK(vkCreateComputePipelines(*m_device, NULL, 1, &info, NULL, &m_handle), "vkCreateComputePipelines fail!");
}

VK_Pipeline::~VK_Pipeline()
{
	if (m_handle != VK_NULL_HANDLE)
		vkDestroyPipeline(*m_device, m_handle, NULL);
}

CU_NS_END
