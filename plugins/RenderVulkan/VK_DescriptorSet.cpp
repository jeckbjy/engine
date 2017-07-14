#include "VK_DescriptorSet.h"
#include "VK_DescriptorSetLayout.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

VK_DescriptorSet::VK_DescriptorSet()
	: m_device(NULL)
	, m_descriptors(VK_NULL_HANDLE)
{
}

VK_DescriptorSet::~VK_DescriptorSet()
{
	term();
}

bool VK_DescriptorSet::init(VK_Device* device, VK_DescriptorSetLayout* layout)
{
	return true;
}

void VK_DescriptorSet::term()
{
	VkDevice device = m_device->getDevice();

	if (m_descriptors != VK_NULL_HANDLE)
	{
		vkFreeDescriptorSets(device, NULL, 1, &m_descriptors);
		m_descriptors = VK_NULL_HANDLE;
	}
}

void VK_DescriptorSet::update()
{
	// 更新数据
}

CUTE_NS_END
