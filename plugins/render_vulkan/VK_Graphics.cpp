#include "VK_Graphics.h"
#include "Util.h"

CU_NS_BEGIN

VK_Graphics::VK_Graphics()
{
	String app_name = Util::exeName();
	// todo:debug layer
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = app_name.c_str();
	app_info.pEngineName = "CuteEngine";

	VkInstanceCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pNext = NULL;
	info.pApplicationInfo = &app_info;
	info.enabledLayerCount = 0;
	info.ppEnabledLayerNames = NULL;
	info.enabledExtensionCount = 0;
	info.ppEnabledExtensionNames = NULL;

	VK_CHECK(vkCreateInstance(&info, NULL, &m_handle), "vkCreateInstance fail!");

	// use first gpu
	VK_CHECK(vkEnumeratePhysicalDevices(m_handle, &m_gpuCount, NULL), "vkEnumeratePhysicalDevices fail!");
	assert(m_gpuCount > 0);
	m_physicals.resize(m_gpuCount);
	VK_CHECK(vkEnumeratePhysicalDevices(m_handle, &m_gpuCount, m_physicals.data()), "vkEnumeratePhysicalDevices fail!");
}

VK_Graphics::~VK_Graphics()
{
	vkDestroyInstance(m_handle, NULL);
}

void VK_Graphics::setPhysical(size_t index)
{
	assert(index < m_physicals.size());
	m_gpu = m_physicals[index];

	// enum
	vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &m_queueCount, NULL);
	//
	vkGetPhysicalDeviceMemoryProperties(m_gpu, &m_memProps);
}

uint32_t VK_Graphics::getMemoryType(uint32_t typeBits, VkFlags properties)
{
	for (uint32_t i = 0; i < 32; ++i)
	{
		if ((typeBits & 1) == 1)
		{
			if ((m_memProps.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
	}

	return 0;
}

CU_NS_END
