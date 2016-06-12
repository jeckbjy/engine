#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class VK_Graphics;
class CU_VK_API VK_Device : public Device
{
	CU_NONCOPYABLE(VK_Device);
public:
	VK_Device();
	virtual ~VK_Device();

	void allocMemory(VkDeviceMemory& memory, uint32_t bytes, uint32_t typeBits, VkFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	VkDevice& native() { return m_device; }
	VkDescriptorPool& getDescriptorPool() { return m_descroptorPool; }

private:
	void createInstance();
	void createPhysical();
	void createDevice();

private:
	VkInstance			m_instance;
	VkPhysicalDevice	m_physical;
	VkDevice			m_device;
	VkDescriptorPool	m_descroptorPool;
	uint32_t			m_queueIndex;
	VkPhysicalDeviceMemoryProperties m_memoryProps;

};

CU_NS_END
