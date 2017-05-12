#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_Sampler : public ISampler
{
public:
	VK_Sampler();
	~VK_Sampler();

	bool init(VK_Device* device, const SamplerDesc& desc);
	void term();

private:
	VK_Device*	m_device;
	VkSampler	m_sampler;
};

CUTE_NS_END
