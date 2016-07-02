#include "VK_SwapChain.h"
#include "Window.h"

CU_NS_BEGIN

VK_SwapChain::VK_SwapChain(const SwapChainDesc& desc)
	: m_wnd(desc.wnd)
{

}

VK_SwapChain::~VK_SwapChain()
{

}

void VK_SwapChain::createSurface()
{
	VkInstance instance = NULL;
	VkResult err;
	window_t handle = m_wnd->getHandle();
#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.hinstance = NULL;
	info.hwnd = handle;
	err = vkCreateWin32SurfaceKHR(instance, &info, NULL, &m_surface);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	VkAndroidSurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.window = handle;
	err = vkCreateAndroidSurfaceKHR(instance, &info, NULL, &m_surface);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	VkXcbSurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.connection = NULL;
	info.window = NULL;
	err = vkCreateXcbSurfaceKHR(instance, &info, NULL, &m_surface);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	VkXlibSurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.dpy = NULL;
	info.window = handle;
	err = vkCreateXlibSurfaceKHR(instance, &info, NULL, &m_surface);
#endif
}

void VK_SwapChain::createChain()
{
	VkSwapchainCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = m_surface;
	info.minImageCount = 0;
	info.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
	info.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	info.imageExtent = { 0, 0 };
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
}

void VK_SwapChain::present()
{

}

void VK_SwapChain::bind(void*)
{

}

CU_NS_END