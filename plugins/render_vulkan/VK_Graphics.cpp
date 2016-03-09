#include "VK_Graphics.h"

CU_NS_BEGIN

VKGraphics::VKGraphics()
{
	VkInstanceCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pNext = NULL;
	info.pApplicationInfo = NULL;
	info.enabledLayerCount = 0;
	info.ppEnabledLayerNames = NULL;
	info.enabledExtensionCount = 0;
	info.ppEnabledExtensionNames = NULL;

	VK_CHECK(vkCreateInstance(&info, NULL, &m_handle));

	// use first gpu
	uint32_t gpu_count;
	VK_CHECK(vkEnumeratePhysicalDevices(m_handle, &gpu_count, NULL));
	assert(gpu_count > 0);
	VkPhysicalDevice* gpu_array = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
	VK_CHECK(vkEnumeratePhysicalDevices(m_handle, &gpu_count, gpu_array));
	m_gpu = gpu_array[0];
	free(gpu_array);
}

VKGraphics::~VKGraphics()
{
	vkDestroyInstance(m_handle, NULL);
}

CU_NS_END
