#include "VK_SwapChain.h"
//#include "Window.h"

CUTE_NS_BEGIN

VK_SwapChain::VK_SwapChain()
	: m_device(NULL)
	, m_queue(NULL)
	, m_surface(VK_NULL_HANDLE)
	, m_swapchain(VK_NULL_HANDLE)
	, m_buffers(NULL)
	, m_images(NULL)
	, m_views(NULL)
{
}

VK_SwapChain::~VK_SwapChain()
{
	term();
}

bool VK_SwapChain::init(VK_Device* device, const SwapChainDesc& desc)
{
	VkDevice nativeDevice = device->getDevice();
	VkPhysicalDevice nativePhysical = device->getPhysicalDevice();

	m_device = device;
	m_device->incRef();

	m_device->getGraphicsQueue(&m_queue);

	// init surface
	if (!initSurface())
		return false;
	//
	m_format = VK_FORMAT_UNDEFINED;
	uint32_t familyIndex = m_queue->getFamilyIndex();
	VkBool32 surrpot = VK_FALSE;
	VkResult ret = vkGetPhysicalDeviceSurfaceSupportKHR(nativePhysical, familyIndex, m_surface, &surrpot);
	if (ret != VK_SUCCESS || surrpot == VK_FALSE)
		return false;

	uint32_t count = 0;
	ret = vkGetPhysicalDeviceSurfaceFormatsKHR(nativePhysical, m_surface, &count, NULL);
	if (ret != VK_SUCCESS)
		return false;


}

bool VK_SwapChain::initSurface()
{
	VkDevice device = m_device->getDevice();
	VkResult result;

#if defined(VK_USE_PLATFORM_WIN32_KHR)
	VkWin32SurfaceCreateInfoKHR info = {};
    info.sType      = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.pNext      = nullptr;
    info.flags      = 0;
    info.hinstance  = NULL;
    info.hwnd       = NULL;

	result = vkCreateWin32SurfaceKHR(device, &info, NULL, &m_surface);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	VkAndroidSurfaceCreateInfoKHR info = {};
	info.sType		= VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	info.pNext		= NULL;
	info.flags		= 0;
	info.window		= NULL;

	result = vkCreateAndroidSurfaceKHR(device, &info, NULL, &m_surface);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	VkXlibSurfaceCreateInfoKHR info = {};

#elif defined(VK_USE_PLATFORM_XCB_KHR)
	VkXcbSurfaceCreateInfoKHR info = {};
	info.sType		= VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	info.pNext		= NULL;
	info.flags		= 0;
	info.connection = static_cast<xcb_connection_t*>(m_Desc.InstanceHandle);
	info.windows	= static_cast<xcb_window_t>(m_Desc.WindowHandle);

	result = vkCreateXcbSurfaceKHR(device, &info, NULL, &m_surface);

#elif defined(VK_USE_PLATFORM_VI_NN)
	VkViSurfaceCreateInfoNN info = {};
	info.sType		= VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN;
	info.pNext		= NULL;
	info.flags		= 0;
	info.window		= m_Desc.WindowHandle;

	result = = vkCreateViSurfaceNN(pNativeInstance, &info, NULL, &m_surface);

#endif

	return result == VK_SUCCESS;
}

void VK_SwapChain::term()
{

}

void VK_SwapChain::present()
{
	VkPresentInfoKHR info = {};
    info.sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext          = NULL;
    info.swapchainCount = 1;
    info.pSwapchains    = &m_swapchain;
    info.pImageIndices  = &m_currIndex;

	VkDevice device = m_device->getDevice();

	VkSemaphore semapthore = m_queue->getWaitSemapthore();
	VkFence		fence = m_queue->getFence();

	uint64_t infinite = 0xFFFFFFFF;
	VkResult result;

	result = vkQueuePresentKHR(m_queue->getQueue(), &info);
	assert(result == VK_SUCCESS);

	result = vkAcquireNextImageKHR(device, m_swapchain, infinite, semapthore, fence, &m_currIndex);
	assert(result == VK_SUCCESS);
	
	vkWaitForFences(device, 1, &fence, VK_FALSE, infinite);
	vkResetFences(device, 1, &fence);
}

bool VK_SwapChain::resize(uint32 width, uint32 height)
{
	return true;
}

bool VK_SwapChain::getBuffer(uint32 index, ITexture* texture)
{
	return true;
}

bool VK_SwapChain::setMetaData(META_DATA_TYPE type, void* data)
{
	return true;
}

void VK_SwapChain::setFullScreen(bool enable)
{
}

bool VK_SwapChain::isFullScreen() const
{
	return true;
}

bool VK_SwapChain::checkColorSpaceSupport(COLOR_SPACE_TYPE type, uint32* flags)
{
	return true;
}

//VK_SwapChain::VK_SwapChain(const SwapChainDesc& desc)
//	: m_wnd(desc.wnd)
//{
//
//}
//
//VK_SwapChain::~VK_SwapChain()
//{
//
//}
//
//void VK_SwapChain::createSurface()
//{
//	VkInstance instance = NULL;
//	VkResult err;
//	window_t handle = m_wnd->getHandle();
//#ifdef VK_USE_PLATFORM_WIN32_KHR
//	VkWin32SurfaceCreateInfoKHR info = {};
//	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//	info.hinstance = NULL;
//	info.hwnd = handle;
//	err = vkCreateWin32SurfaceKHR(instance, &info, NULL, &m_surface);
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//	VkAndroidSurfaceCreateInfoKHR info = {};
//	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//	info.window = handle;
//	err = vkCreateAndroidSurfaceKHR(instance, &info, NULL, &m_surface);
//#elif defined(VK_USE_PLATFORM_XCB_KHR)
//	VkXcbSurfaceCreateInfoKHR info = {};
//	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//	info.connection = NULL;
//	info.window = NULL;
//	err = vkCreateXcbSurfaceKHR(instance, &info, NULL, &m_surface);
//#elif defined(VK_USE_PLATFORM_XLIB_KHR)
//	VkXlibSurfaceCreateInfoKHR info = {};
//	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//	info.dpy = NULL;
//	info.window = handle;
//	err = vkCreateXlibSurfaceKHR(instance, &info, NULL, &m_surface);
//#endif
//}
//
//void VK_SwapChain::createChain()
//{
//	VkSwapchainCreateInfoKHR info = {};
//	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//	info.surface = m_surface;
//	info.minImageCount = 0;
//	info.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
//	info.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
//	info.imageExtent = { 0, 0 };
//	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//	info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
//}
//
//void VK_SwapChain::present()
//{
//
//}
//
//void VK_SwapChain::bind(void*)
//{
//
//}

CUTE_NS_END
