#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_Device : public Device
{
	CU_NONCOPYABLE(VK_Device);
public:
	VK_Device(VK_Graphics* graphics);
	virtual ~VK_Device();

	void allocMemory(VkDeviceMemory& memory, uint32_t bytes, uint32_t typeBits, VkFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	VkDevice& handle() { return m_handle; }

	operator VkDevice() const { return m_handle; }

private:
	VkDevice		m_handle;
	//VkQueue			m_queue;
	VK_Graphics*		m_graphics;
};

CU_NS_END
