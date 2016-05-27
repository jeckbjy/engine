#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class VK_Device;
class VK_CommandBuffer;
class CU_VK_API VK_Pipeline : public Pipeline
{
public:
	virtual void bind(VK_CommandBuffer* cmdBuffer) = 0;
};

// �������⣺1:VertexInputState��ʼ��,2:RenderPass�ĳ�ʼ��,3:Layout�ĳ�ʼ��
class CU_VK_API VK_GraphicsPipeline : public Pipeline
{
public:
	VK_GraphicsPipeline(VkDevice device, const PipelineDesc& desc);
	~VK_GraphicsPipeline();

	void bind(VK_CommandBuffer* cmdBuffer);
	//VkPipeline getPipeline(InputLayout* layout);

private:
	// todo:�Ƿ���Ҫ ����RenderPass
	typedef std::map<uint32_t, VkPipeline> PipelineMap;
	PipelineMap								m_handles;
	VkDevice								m_device;

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
	VK_ComputePipeline(VkDevice device, const PipelineDesc& desc);
	~VK_ComputePipeline();

	void bind(VK_CommandBuffer* cmdBuffer);

private:
	VkDevice	m_device;
	VkPipeline	m_handle;
};

CU_NS_END
