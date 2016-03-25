#include "VK_Pipeline.h"
#include "VK_Mapping.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_GraphicsPipeline::VK_GraphicsPipeline(VK_Device* device, const GraphicsPipelineDesc& desc)
	: m_device(device)
{
	VK_Mapping::fillInputAssemblyState(m_inputAssemblyState, desc.topology);
	VK_Mapping::fillTessellationState(m_tessellationState, desc.topology);
	VK_Mapping::fillRasterizationState(m_rasterizationState, desc.rasterizer);
	VK_Mapping::fillMultisampleState(m_multisampleState, desc.multisample);
	VK_Mapping::fillDepthStencilState(m_depthStencilState, desc.depthStencil);
	VK_Mapping::fillBlendState(m_blendState, m_blendTarget, desc.blend);
	VK_Mapping::fillViewportState(m_viewportState);

	//
	m_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	m_info.pNext = NULL;
	m_info.flags = 0;
	// todo:
	m_info.stageCount = 0;
	m_info.pStages = NULL;

	m_info.pVertexInputState = NULL;	// 创建
	m_info.pInputAssemblyState = &m_inputAssemblyState;
	m_info.pTessellationState = &m_tessellationState;
	m_info.pViewportState = &m_viewportState;
	m_info.pRasterizationState = &m_rasterizationState;
	m_info.pMultisampleState = &m_multisampleState;
	m_info.pDepthStencilState = &m_depthStencilState;
	m_info.pColorBlendState = &m_blendState;
	m_info.pDynamicState = &m_dynamicState;
	// 如何初始化？
	m_info.layout = 0;
	m_info.renderPass = 0;
	m_info.subpass = 0;
	m_info.basePipelineHandle = 0;
	m_info.basePipelineIndex = 0;

	//	VK_CHECK(vkCreateGraphicsPipelines(m_device->native(), NULL, 1, &info, NULL, &m_handle), "vkCreateGraphicsPipelines fail!");

}

VK_GraphicsPipeline::~VK_GraphicsPipeline()
{
	// 依次释放
	for (PipelineMap::iterator itor = m_handles.begin(); itor != m_handles.end(); ++itor)
	{
		vkDestroyPipeline(m_device->native(), itor->second, NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
VK_ComputePipeline::VK_ComputePipeline(VK_Device* device, const ComputePipelineDesc& desc)
	: m_device(device)
	, m_handle(VK_NULL_HANDLE)
{
	VkComputePipelineCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	// fill shader info
	info.layout = NULL;	// how to fill??
	info.basePipelineHandle = VK_NULL_HANDLE;
	info.basePipelineIndex = 0;

	VK_CHECK(vkCreateComputePipelines(m_device->native(), NULL, 1, &info, NULL, &m_handle), "vkCreateComputePipelines fail!");
}

VK_ComputePipeline::~VK_ComputePipeline()
{
	if (m_handle)
	{
		vkDestroyPipeline(m_device->native(), m_handle, NULL);
		m_handle = NULL;
	}
}

CU_NS_END
