#include "VK_Pipeline.h"
#include "VK_Device.h"
#include "VK_Mapping.h"
#include "VK_CommandList.h"
#include "VK_FrameBuffer.h"
#include "VK_DescriptorSetLayout.h"

CUTE_NS_BEGIN

VK_Pipeline::VK_Pipeline()
	: m_device(NULL)
	, m_pipeline(VK_NULL_HANDLE)
	, m_bindpoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
	, m_cache(VK_NULL_HANDLE)
{
}

VK_Pipeline::~VK_Pipeline()
{
	term();
}

bool VK_Pipeline::init(VK_Device* device, const GraphicsPipelineDesc& desc)
{
	VkDevice native = device->getDevice();
	m_device = device;

	VK_DescriptorSetLayout* pWrapLayout = (VK_DescriptorSetLayout*)desc.layout;
	VK_FrameBuffer*			pWrapFrameBuffer = (VK_FrameBuffer*)desc.framebuffer;

	m_bindpoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	// cache

	// create
	VkPipelineShaderStageCreateInfo         shaderInfos[5] = {};
	VkVertexInputAttributeDescription*      pInputAttrs = NULL;
	VkVertexInputBindingDescription*        pBindingDescs = NULL;
	VkPipelineVertexInputStateCreateInfo    vertexInputState = {};
	VkPipelineInputAssemblyStateCreateInfo  inputAssemblyState = {};
	VkPipelineTessellationStateCreateInfo   tessellationState = {};
	VkPipelineViewportStateCreateInfo       viewportState = {};
	VkPipelineRasterizationStateCreateInfo  rasterizerState = {};
	VkPipelineMultisampleStateCreateInfo    multisampleState = {};
	VkPipelineDepthStencilStateCreateInfo   depthStencilState = {};
	VkPipelineColorBlendAttachmentState     colorAttachments[8] = {};
	VkPipelineColorBlendStateCreateInfo     colorBlendState = {};
	uint32_t shaderCount = 0;

	VkDynamicState dynamicElements[] = 
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_BLEND_CONSTANTS,
		VK_DYNAMIC_STATE_STENCIL_REFERENCE
	};

	VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType              = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext              = nullptr;
    dynamicState.flags              = 0;
    dynamicState.dynamicStateCount  = 4;
    dynamicState.pDynamicStates     = dynamicElements;

	VkGraphicsPipelineCreateInfo info = {};
    info.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext                  = NULL;
    info.flags                  = 0;
    info.stageCount             = shaderCount;
    info.pStages                = shaderInfos;
    info.pVertexInputState      = &vertexInputState;
    info.pInputAssemblyState    = &inputAssemblyState;
    info.pTessellationState     = &tessellationState;
    info.pViewportState         = &viewportState;
    info.pRasterizationState    = &rasterizerState;
    info.pMultisampleState      = &multisampleState;
    info.pDepthStencilState     = &depthStencilState;
    info.pColorBlendState       = &colorBlendState;
    info.pDynamicState          = &dynamicState;
    info.layout                 = pWrapLayout->getPipelineLayout();
    //info.renderPass             = pWrapFrameBuffer->GetRenderPass();
    info.subpass                = 0;
    info.basePipelineHandle     = VK_NULL_HANDLE;
    info.basePipelineIndex      = 0;

	VkResult ret = vkCreateGraphicsPipelines(native, m_cache, 1, &info, NULL, &m_pipeline);

	return ret == VK_SUCCESS;
}

bool VK_Pipeline::init(VK_Device* device, const ComputePipelineDesc& desc)
{
	VkDevice native;
	VkResult result;
	m_device = device;
	native = device->getDevice();

	VkPipelineCacheCreateInfo cacheInfo = {};
	cacheInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cacheInfo.pNext				= NULL;
	cacheInfo.flags				= 0;
	cacheInfo.initialDataSize	= 0;
	cacheInfo.pInitialData		= NULL;

	result = vkCreatePipelineCache(native, &cacheInfo, NULL, &m_cache);
	if (result != VK_SUCCESS)
		return false;

	VK_DescriptorSetLayout* pWrapLayout = static_cast<VK_DescriptorSetLayout*>(desc.layout);

	VkComputePipelineCreateInfo info = {};
	info.sType				= VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	info.pNext				= NULL;
	info.flags				= 0;
	info.layout				= pWrapLayout->getPipelineLayout();
	info.basePipelineHandle = VK_NULL_HANDLE;
	info.basePipelineIndex	= 0;

	result = vkCreateComputePipelines(native, m_cache, 1, &info, NULL, &m_pipeline);

	return result == VK_SUCCESS;
}

void VK_Pipeline::term()
{
	if (m_device == NULL)
		return;

	VkDevice device = m_device->getDevice();

	if (m_pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(device, m_pipeline, NULL);
		m_pipeline = VK_NULL_HANDLE;
	}

	if (m_cache != VK_NULL_HANDLE)
	{
		vkDestroyPipelineCache(device, m_cache, NULL);
		m_cache = VK_NULL_HANDLE;
	}

	m_device = NULL;
}


//VK_GraphicsPipeline::VK_GraphicsPipeline(VkDevice device, const PipelineDesc& desc)
//	: m_device(device)
//{
//	const RenderStateDesc* stateDesc = desc.states;
//
//	VK_Mapping::fillInputAssemblyState(m_inputAssemblyState, stateDesc->topology);
//	VK_Mapping::fillTessellationState(m_tessellationState, stateDesc->topology);
//	VK_Mapping::fillRasterizationState(m_rasterizationState, stateDesc->rasterizer);
//	VK_Mapping::fillMultisampleState(m_multisampleState, stateDesc->multisample);
//	VK_Mapping::fillDepthStencilState(m_depthStencilState, stateDesc->depthStencil);
//	VK_Mapping::fillBlendState(m_blendState, m_blendTarget, stateDesc->blend);
//	VK_Mapping::fillViewportState(m_viewportState);
//
//	//
//	m_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//	m_info.pNext = NULL;
//	m_info.flags = 0;
//	// todo: fill shader info
//	m_info.stageCount = 0;
//	m_info.pStages = NULL;
//
//	m_info.pVertexInputState = NULL;	// 创建
//	m_info.pInputAssemblyState = &m_inputAssemblyState;
//	m_info.pTessellationState = &m_tessellationState;
//	m_info.pViewportState = &m_viewportState;
//	m_info.pRasterizationState = &m_rasterizationState;
//	m_info.pMultisampleState = &m_multisampleState;
//	m_info.pDepthStencilState = &m_depthStencilState;
//	m_info.pColorBlendState = &m_blendState;
//	m_info.pDynamicState = &m_dynamicState;
//	// 如何初始化？
//	m_info.layout = 0;
//	m_info.renderPass = 0;
//	m_info.subpass = 0;
//	m_info.basePipelineHandle = 0;
//	m_info.basePipelineIndex = 0;
//}
//
//VK_GraphicsPipeline::~VK_GraphicsPipeline()
//{
//	// 依次释放
//	for (PipelineMap::iterator itor = m_handles.begin(); itor != m_handles.end(); ++itor)
//	{
//		vkDestroyPipeline(m_device, itor->second, NULL);
//	}
//}
//
//void VK_GraphicsPipeline::bind(VK_CommandList* cmdBuffer)
//{
//	VK_VertexLayout* layout = cmdBuffer->getLayout();
//	if (!layout)
//		return;
//
//	// 先查找,render pass?
//
//	m_info.pVertexInputState = layout->getInfo();
//	VkPipeline handle = NULL;
//	VK_CHECK(vkCreateGraphicsPipelines(m_device, NULL, 1, &m_info, NULL, &handle), "vkCreateGraphicsPipelines fail!");
//
//	// 绑定
//	vkCmdBindPipeline(cmdBuffer->getNative(), VK_PIPELINE_BIND_POINT_GRAPHICS, handle);
//}
//
////////////////////////////////////////////////////////////////////////////
////
////////////////////////////////////////////////////////////////////////////
//VK_ComputePipeline::VK_ComputePipeline(VkDevice device, const PipelineDesc& desc)
//	: m_device(device)
//	, m_handle(VK_NULL_HANDLE)
//{
//	VkComputePipelineCreateInfo info;
//	info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
//	info.pNext = NULL;
//	info.flags = 0;
//	// fill shader info
//	info.layout = NULL;	// how to fill??
//	info.basePipelineHandle = VK_NULL_HANDLE;
//	info.basePipelineIndex = 0;
//
//	VK_CHECK(vkCreateComputePipelines(m_device, NULL, 1, &info, NULL, &m_handle), "vkCreateComputePipelines fail!");
//}
//
//VK_ComputePipeline::~VK_ComputePipeline()
//{
//	if (m_handle)
//	{
//		vkDestroyPipeline(m_device, m_handle, NULL);
//		m_handle = NULL;
//	}
//}
//
//void VK_ComputePipeline::bind(VK_CommandList* cmdBuffer)
//{
//	vkCmdBindPipeline(cmdBuffer->getNative(), VK_PIPELINE_BIND_POINT_COMPUTE, m_handle);
//}

CUTE_NS_END
