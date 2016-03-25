#pragma once
#include "VK_API.h"

CU_NS_BEGIN

// �������⣺1:VertexInputState��ʼ��,2:RenderPass�ĳ�ʼ��,3:Layout�ĳ�ʼ��
class CU_VK_API VK_GraphicsPipeline : public Pipeline
{
public:
	VK_GraphicsPipeline(VK_Device* device, const GraphicsPipelineDesc& desc);
	~VK_GraphicsPipeline();

	//VkPipeline getPipeline(InputLayout* layout);

private:
	// todo:�Ƿ���Ҫ ����RenderPass
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
	VkPipelineLayout						m_layout;				// ��δ�������
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
