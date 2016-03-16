#include "VK_Pipeline.h"
#include "VK_Mapping.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_Pipeline::VK_Pipeline(VK_Device* device, const GraphicsPipelineDesc& desc)
	: m_device(device)
	, m_handle(VK_NULL_HANDLE)
{
	VkPipelineShaderStageCreateInfo stages[5];

	VkGraphicsPipelineCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.pNext = NULL;
	// convert
	info.flags = 0;

	// 拓扑类型
	VkPipelineInputAssemblyStateCreateInfo ia_info;
	ia_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	ia_info.pNext = NULL;
	ia_info.topology = VK_Mapping::getTopology(desc.topology);
	ia_info.primitiveRestartEnable = false;

	// 萃取反射or后期设置？？
	typedef std::vector<VkVertexInputBindingDescription>	BindDescVec;
	typedef std::vector<VkVertexInputAttributeDescription>	AttrDescVec;

	VkPipelineVertexInputStateCreateInfo vi_info;
	vi_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vi_info.pNext = NULL;

	//
	VkPipelineRasterizationStateCreateInfo rast_info;
	VkPipelineMultisampleStateCreateInfo multisample_info;
	VkPipelineDepthStencilStateCreateInfo ds_info;
	VkPipelineColorBlendStateCreateInfo blend_info;
	VkPipelineColorBlendAttachmentState blend_targets[8];

	VkPipelineTessellationStateCreateInfo tess_info;
	VkPipelineViewportStateCreateInfo viewport_info;

	VK_Mapping::fillRasterizationState(rast_info, desc.rasterizer);
	VK_Mapping::fillMultisampleState(multisample_info, desc.multisample);
	VK_Mapping::fillDepthStencilState(ds_info, desc.depthStencil);
	VK_Mapping::fillBlendState(blend_info, blend_targets,desc.blend);

	info.pVertexInputState = &vi_info;
	info.pInputAssemblyState = &ia_info;
	info.pTessellationState = &tess_info;
	info.pViewportState = &viewport_info;
	info.pRasterizationState = &rast_info;
	info.pMultisampleState = &multisample_info;
	info.pDepthStencilState = &ds_info;
	info.pColorBlendState = &blend_info;
	info.renderPass = VK_NULL_HANDLE;		// todo:search

	VK_CHECK(vkCreateGraphicsPipelines(m_device->native(), NULL, 1, &info, NULL, &m_handle), "vkCreateGraphicsPipelines fail!");
}

VK_Pipeline::VK_Pipeline(VK_Device* device, const ComputePipelineDesc& desc)
	: m_device(device)
	, m_handle(VK_NULL_HANDLE)
{
	VkComputePipelineCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	info.pNext = NULL;

	VK_CHECK(vkCreateComputePipelines(m_device->native(), NULL, 1, &info, NULL, &m_handle), "vkCreateComputePipelines fail!");
}

VK_Pipeline::~VK_Pipeline()
{
	if (m_handle != VK_NULL_HANDLE)
		vkDestroyPipeline(m_device->native(), m_handle, NULL);
}

CU_NS_END
