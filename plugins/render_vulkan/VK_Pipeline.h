#pragma once
#include "VK_API.h"

CU_NS_BEGIN

// 三个问题：1:VertexInputState初始化,2:RenderPass的初始化,3:Layout的初始化
class CU_VK_API VK_GraphicsPipeline : public Pipeline
{
public:
	VK_GraphicsPipeline(VK_Device* device, const GraphicsPipelineDesc& desc);
	~VK_GraphicsPipeline();

	//VkPipeline getPipeline(InputLayout* layout);

private:
	// todo:是否还需要 加入RenderPass
	typedef std::map<uint32_t, VkPipeline> PipelineMap;
	PipelineMap m_handles;
	VK_Device*	m_device;
	VkGraphicsPipelineCreateInfo			m_info;
	VkPipelineInputAssemblyStateCreateInfo	m_inputAssemblyState;
	VkPipelineTessellationStateCreateInfo	m_tessellationState;
	VkPipelineViewportStateCreateInfo		m_viewportState;
	VkPipelineRasterizationStateCreateInfo	m_rasterizationState;
	VkPipelineMultisampleStateCreateInfo	m_multisampleState;
	VkPipelineDepthStencilStateCreateInfo	m_depthStencilState;
	VkPipelineColorBlendStateCreateInfo		m_blendState;
	VkPipelineColorBlendAttachmentState		m_blendTarget[8];
	VkPipelineDynamicStateCreateInfo		m_dynamicState;
	VkPipelineLayout						m_layout;				// 如何创建？？
};

class CU_VK_API VK_ComputePipeline : public Pipeline
{
public:
	VK_ComputePipeline(VK_Device* device, const ComputePipelineDesc& desc);
	~VK_ComputePipeline();

private:
	VK_Device*	m_device;
	VkPipeline	m_handle;
};

CU_NS_END
