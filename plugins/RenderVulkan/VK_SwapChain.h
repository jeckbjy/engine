#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class VK_CommandQueue;
class VK_Texture;
class CUTE_VK_API VK_SwapChain : public ISwapChain
{
public:
	VK_SwapChain();
	~VK_SwapChain();

	bool init(VK_Device* device, const SwapChainDesc& desc);
	void term();

	void present() OVERRIDE;
	bool resize(uint32 width, uint32 height) OVERRIDE;
	bool getBuffer(uint32 index, ITexture* texture) OVERRIDE;
	bool setMetaData(META_DATA_TYPE type, void* data) OVERRIDE;
	void setFullScreen(bool enable) OVERRIDE;
	bool isFullScreen() const OVERRIDE;
	bool checkColorSpaceSupport(COLOR_SPACE_TYPE type, uint32* flags) OVERRIDE;

private:
	bool initSurface();

private:
	VK_Device*						m_device;
	VK_CommandQueue*				m_queue;
	VkSurfaceKHR					m_surface;
	VkSwapchainKHR					m_swapchain;
	VkPresentModeKHR				m_presentMode;
	VkFormat						m_format;
	VkColorSpaceKHR					m_colorSpace;
	VkSurfaceTransformFlagBitsKHR	m_transform;
	VK_Texture**					m_buffers;
	VkImage*						m_images;
	VkImageView*					m_views;
	uint32							m_currIndex;
};

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

//class CUTE_VK_API VK_SwapChain : public ISwapChain
//{
//public:
//	VK_SwapChain(const SwapChainDesc& desc);
//	~VK_SwapChain();
//
//	void present();
//	void bind(void*);
//
//private:
//	void createSurface();
//	void createChain();
//
//private:
//	Window*			m_wnd;
//	VkSurfaceKHR	m_surface;
//	VkSwapchainKHR	m_chain;
//};

CUTE_NS_END
