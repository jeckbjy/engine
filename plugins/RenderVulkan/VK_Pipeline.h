#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_Pipeline : public IPipeline
{
public:
	VK_Pipeline();
	~VK_Pipeline();

	bool init(VK_Device* device, const GraphicsPipelineDesc& desc);
	bool init(VK_Device* device, const ComputePipelineDesc& desc);
	void term();

private:
	VK_Device*			m_device;
	VkPipeline			m_pipeline;
	VkPipelineBindPoint	m_bindpoint;
	VkPipelineCache		m_cache;
};

//class VK_Device;
//class VK_CommandList;
//class CUTE_VK_API VK_Pipeline : public IPipeline
//{
//public:
//	virtual void bind(VK_CommandList* cmdBuffer) = 0;
//};
//
//// 三个问题：1:VertexInputState初始化,2:RenderPass的初始化,3:Layout的初始化
//class CUTE_VK_API VK_GraphicsPipeline : public IPipeline
//{
//public:
//	VK_GraphicsPipeline(VkDevice device, const PipelineDesc& desc);
//	~VK_GraphicsPipeline();
//
//	void bind(VK_CommandList* cmdBuffer);
//	//VkPipeline getPipeline(InputLayout* layout);
//
//private:
//	// todo:是否还需要 加入RenderPass
//	typedef std::map<uint32_t, VkPipeline> PipelineMap;
//	PipelineMap								m_handles;
//	VkDevice								m_device;
//
//	VkGraphicsPipelineCreateInfo			m_info;
//	VkPipelineInputAssemblyStateCreateInfo	m_inputAssemblyState;
//	VkPipelineTessellationStateCreateInfo	m_tessellationState;
//	VkPipelineViewportStateCreateInfo		m_viewportState;
//	VkPipelineRasterizationStateCreateInfo	m_rasterizationState;
//	VkPipelineMultisampleStateCreateInfo	m_multisampleState;
//	VkPipelineDepthStencilStateCreateInfo	m_depthStencilState;
//	VkPipelineColorBlendStateCreateInfo		m_blendState;
//	VkPipelineColorBlendAttachmentState		m_blendTarget[8];
//	VkPipelineDynamicStateCreateInfo		m_dynamicState;
//	VkPipelineLayout						m_layout;				// 如何创建？？
//};
//
//class CUTE_VK_API VK_ComputePipeline : public IPipeline
//{
//public:
//	VK_ComputePipeline(VkDevice device, const PipelineDesc& desc);
//	~VK_ComputePipeline();
//
//	void bind(VK_CommandList* cmdBuffer);
//
//private:
//	VkDevice	m_device;
//	VkPipeline	m_handle;
//};

CUTE_NS_END
