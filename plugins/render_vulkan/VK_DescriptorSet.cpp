#include "VK_DescriptorSet.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_DescriptroSet::VK_DescriptroSet(VK_Device* device)
	: m_device(device)
	, m_descriptors(VK_NULL_HANDLE)
{
	VkDescriptorSetAllocateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.descriptorPool = device->getDescriptorPool();
	info.descriptorSetCount = 1;
	info.pSetLayouts = &m_layout;
	vkAllocateDescriptorSets(m_device->native(), &info, &m_descriptors);
}

VK_DescriptroSet::~VK_DescriptroSet()
{
	vkDestroyDescriptorSetLayout(m_device->native(), m_layout, NULL);
}

CU_NS_END
