#pragma once
#include "VK_API.h"

CU_NS_BEGIN

//#if defined(VK_USE_PLATFORM_WIN32_KHR)
//typedef VkWin32SurfaceCreateInfoKHR		surface_t;
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//typedef VkAndroidSurfaceCreateInfoKHR	surface_t;
//#elif defined(VK_USE_PLATFORM_XCB_KHR)
//typedef VkXcbSurfaceCreateInfoKHR		surface_t;
//#elif defined(VK_USE_PLATFORM_XLIB_KHR)
//typedef VkXlibSurfaceCreateInfoKHR		surface_t;
//#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
//typedef VkWaylandSurfaceCreateInfoKHR	surface_t;
//#elif defined(VK_USE_PLATFORM_MIR_KHR)
//typedef VkMirSurfaceCreateInfoKHR		surface_t;
//#endif

class CU_VK_API VK_SwapChain : public SwapChain
{
public:
	VK_SwapChain(Window* wnd);
	~VK_SwapChain();

	void present(){}

private:
	void createSurface();
	void createChain();

private:
	Window*			m_wnd;
	VkSurfaceKHR	m_surface;
	VkSwapchainKHR	m_chain;
};

CU_NS_END