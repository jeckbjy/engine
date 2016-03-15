#include "VK_Device.h"
#include "VK_Graphics.h"

CU_NS_BEGIN

VK_Device::VK_Device(VK_Graphics* graphics)
	: m_graphics(graphics)
{
	VkDeviceQueueCreateInfo queue_info = 
	{
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		NULL,
		0,
		0,
		0,
		NULL,
	};

	VkDeviceCreateInfo info = 
	{
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		NULL,
		0,
		1,
		&queue_info,
		0, NULL,
		0, NULL,
		NULL
	};

	VK_CHECK(vkCreateDevice(m_graphics->getPhysicalDevice(), &info, NULL, &m_handle), "vkCreateDevice fail!");

	// create pool
	VkDescriptorPoolSize typeCounts[1];
	//typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//typeCounts[0].descriptorCount = 1;

	VkDescriptorPoolCreateInfo pool_info;
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.pNext = NULL;
	pool_info.poolSizeCount = 1;
	pool_info.pPoolSizes = typeCounts;
	pool_info.maxSets = 1;
	VK_CHECK(vkCreateDescriptorPool(m_handle, &pool_info, NULL, &m_descroptorPool), "vkCreateDescriptorPool fail!");

}

VK_Device::~VK_Device()
{
	vkDestroyDevice(m_handle, NULL);
}

void VK_Device::allocMemory(VkDeviceMemory& memory, uint32_t bytes, uint32_t typeBits, VkFlags properties)
{
	VkMemoryAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.pNext = NULL;
	info.allocationSize = bytes;
	info.memoryTypeIndex = m_graphics->getMemoryType(typeBits, properties);

	VK_CHECK(vkAllocateMemory(m_handle, &info, NULL, &memory), "vkAllocateMemory fail!");
}

CU_NS_END
