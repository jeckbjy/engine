#include "VK_Device.h"
#include "VK_Graphics.h"

CU_NS_BEGIN

VKDevice::VKDevice()
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
	VK_CHECK(vkCreateDevice(VKGraphics::instance().gpu(), &info, NULL, &m_handle));
}

VKDevice::~VKDevice()
{
	vkDestroyDevice(m_handle, NULL);
}

CU_NS_END