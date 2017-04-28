#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

// ����
class VK_Device;
class CU_VK_API VK_DescriptroSet : public DescriptorSet
{
public:
	VK_DescriptroSet(VK_Device* device);
	~VK_DescriptroSet();

private:
	VK_Device*				m_device;
	VkDescriptorSet			m_descriptors;
	VkDescriptorSetLayout	m_layout;
};

CUTE_NS_END
