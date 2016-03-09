#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKPipeline : public Pipeline
{
public:
	VKPipeline();
	~VKPipeline();

	VkPipelineBindPoint getBindPoint() const { return VK_PIPELINE_BIND_POINT_GRAPHICS; }

	VkPipeline native() { return m_handle; }

private:
	VkPipeline m_handle;
};

CU_NS_END
