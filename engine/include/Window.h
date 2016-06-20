#pragma once
#include "API.h"
#include "Object.h"

#if defined(CU_OS_WINNT)
#	include <Windows.h>
#elif defined(CU_OS_WINRT)
#	include <agile.h>
#elif defined(CU_OS_ANDROID)
#	include <android/native_window.h>
#elif defined(CU_USE_X11)
#	include <X11/X.h>
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <X11/keysym.h>
#elif defined(CU_USE_XCB)
#	include <xcb/xcb.h>
#endif

CU_NS_BEGIN

// 定义不同类型窗体指针
#if defined(CU_OS_WINNT)
typedef HWND window_t;
#elif defined(CU_OS_WINRT)
typedef Platform::Agile<Windows::UI::Core::CoreWindow> window_t;
#elif defined(CU_OS_ANDROID)
typedef ::ANativeWindow*	window_t;
#elif defined(CU_USE_X11)
typedef ::Window			window_t;
typedef ::Display			display_t;
#elif defined(CU_USE_XCB)
typedef ::xcb_window_t		window_t;
typedef ::xcb_connection_t* connection_t;
#endif

enum WindowBorder
{
	WIN_BORDER_NORMAL,
	WIN_BORDER_NONE,
	WIN_BORDER_FIXED,
};

// 窗体初始化信息
struct WINDOW_DESC
{
	String	title;
	WindowBorder border;
	int32_t left;		// -1=screen center
	int32_t	top;
	size_t	width;
	size_t	height;
	size_t	outputIdx;
	float	refreshRate;
	size_t	vsync;		// 0:not vsync;FPS =refreshRate/vsyncInterval
	size_t	multisample;
	bool	isCustom;
	bool	isFullscreen;
	bool	isHidden;
	bool	isDepthStencil;
	bool	isGamma;
	bool	isDoubleClick;
	bool	isModal;
	bool	isToolWindow;
	bool	isOuterDimensions;
	window_t hwnd;		// 可以挂接到外部已有窗体
	window_t parent;	// 父窗体
	void*	udata;

	WINDOW_DESC()
		: border(WIN_BORDER_NORMAL), left(-1), top(-1), width(1024), height(768), outputIdx(0), refreshRate(60.f)
		, vsync(0), multisample(0)
		, isCustom(true), isFullscreen(false), isHidden(false), isDepthStencil(true), isGamma(false)
		, isDoubleClick(true), isModal(false), isToolWindow(false), isOuterDimensions(false)
		, hwnd(NULL), parent(NULL), udata(NULL)
	{
	}
};

class CU_API Window : public Object
{
	DECLARE_RTTI(Window, Object, OBJ_ID_WINDOW);

private:
	enum WindowFlag
	{
		WF_AUTO_FREE = 0x01,
		WF_HIDDEN = 0x02,
		WF_ACTIVE = 0x04,
		WF_FULLSCREEN = 0x08,
		WF_GAMMA = 0x10,
		WF_MODAL = 0x20,
		WF_DEPTH_STENCIL = 0x40,
		WF_CLOSED = 0x80,
	};

public:
	static bool pollEvents(); // 系统事件循环

public:
	Window(const WINDOW_DESC& desc);
	~Window();

	void resize(size_t width, size_t height);
	void move(int32_t x, int32_t y);

	void minimize();
	void maximize();
	void restore();

	void setHidden(bool hidden);
	void setActive(bool state);
	void setFullscreen();
	void setWindowed();

	void show();
	void hide();

	bool isClosed() const { return (m_flags & WF_CLOSED) != 0; }
	bool isHidden() const { return hasFlag(WF_HIDDEN); }
	bool isActive() const { return hasFlag(WF_ACTIVE); }
	bool isModal() const { return hasFlag(WF_MODAL); }
	bool isFullScreen() const { return hasFlag(WF_FULLSCREEN); }
	bool isMultiSample() const { return m_multisample > 0; }

	window_t handle() { return m_hwnd; }
	int32_t	getLeft() const { return m_left; }
	int32_t getTop() const { return m_top; }
	size_t	getWidth() const { return m_width; }
	size_t	getHeight() const { return m_height; }
	size_t	getMultiSampleCount() const { return m_multisample; }
	size_t	getVsyncInterval() const { return m_vsync; }
	float	getRefreshRate() const { return m_refreshRate; }

	bool hasFlag(WindowFlag mask) const { return (m_flags & mask) == mask; }
	void setFlag(WindowFlag mask, bool flag);

protected:
	window_t m_hwnd;
	void*	 m_udata;	// user data
	uint32	 m_id;		// 唯一id,不能改变
	uint32	 m_flags;	// 位标识
	int32_t	 m_left;
	int32_t	 m_top;
	size_t	 m_width;
	size_t	 m_height;
	size_t	 m_multisample;
	size_t	 m_vsync;
	float	 m_refreshRate;
};

CU_NS_END