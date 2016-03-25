#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_Pipeline : public Pipeline
{
public:
	VK_Pipeline(VK_Device* device, const GraphicsPipelineDesc& desc);
	VK_Pipeline(VK_Device* device, const ComputePipelineDesc& desc);
	~VK_Pipeline();

	VkPipelineBindPoint getBindPoint() const { return VK_PIPELINE_BIND_POINT_GRAPHICS; }

	VkPipeline native() { return m_handle; }

private:
	VK_Device*	m_device;
	VkPipeline	m_handle;
};

class VK_InputLayout;
class CU_VK_API VK_GraphicsPipeline : public Pipeline
{
public:
	VK_GraphicsPipeline(VK_Device* device, const GraphicsPipelineDesc& desc){}

	void bind(VK_InputLayout* layout);

private:
	typedef std::map<uint32_t, VkPipeline> PSOMap;
	// Õ®π˝InputLayout≤È’“Pipeline£ø£ø
	PSOMap m_pipelines;
	VkGraphicsPipelineCreateInfo m_info;
};

class CU_VK_API VK_ComputePipeline : public Pipeline
{
public:
	VK_ComputePipeline(VK_Device* device, const ComputePipelineDesc& desc){}
};

CU_NS_END
