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

CU_NS_END
