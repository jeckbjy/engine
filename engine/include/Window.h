#pragma once
#include "API.h"

#if defined(CU_OS_WINNT)
#	include <Windows.h>
#elif defined(CU_OS_WINRT)
#	include <agile.h>
#elif defined(CU_OS_ANDROID)
#	include <android/native_window.h>
#elif defined(CU_HAS_X11)
#	include <X11/X.h>
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <X11/keysym.h>
#endif

CU_NS_BEGIN

// 定义不同类型窗体指针
#if defined(CU_OS_WINNT)
typedef HWND window_t;
#elif defined(CU_OS_WINRT)
typedef Platform::Agile<Windows::UI::Core::CoreWindow> window_t;
#elif defined(CU_OS_ANDROID)
typedef ::ANativeWindow* window_t;
#elif defined(CU_HAS_X11)
typedef ::Window	window_t;
typedef ::Display	display_t;
#endif

enum WindowFlag
{
	WF_VISIBLE = 0x01,
	WF_RESIZABLE = 0x02,
	WF_FULLSCREEN = 0x04,
	WF_DECORATED = 0x08,	// 标题，系统按钮
	WF_CLOSED = 0x10,		// 关闭
	WF_AUTO_FREE = 0x20,	// 是否需要释放
	WF_GAMMA = 0x40,
	WF_VSYNC = 0x80,
};

class CU_API Window
{
public:
	static window_t create_window(const char* name, size_t w, size_t h, int flags = 0, void* udata = NULL);
	static bool destroy_window(window_t hwnd);
	static Window* current();

public:
	Window(window_t hwnd = NULL);
	~Window();

	bool create(const char* name, size_t w = 1024, size_t h = 768, int flags = WF_VISIBLE);
	void resize(size_t w, size_t h);
	virtual void show();
	virtual void hide();

	virtual void onClose();
	virtual void onActive(bool flag);
	virtual void onResize(size_t w, size_t h);
	virtual void onPaint(){}

	bool isClosed() const { return (m_flags & WF_CLOSED) != 0; }
	bool isVisible() const { return (m_flags & WF_VISIBLE) != 0; }
	bool isFullScreen() const { return (m_flags & WF_FULLSCREEN) != 0; }
	bool isMultisample() const { return false; }

	window_t handle() { return m_hwnd; }
	size_t width() const { return m_width; }
	size_t height() const { return m_height; }

protected:
	window_t m_hwnd;
	uint8_t	 m_flags;	// 位标识
	size_t	 m_width;
	size_t	 m_height;
	void*	 m_udata;	// user data
};

CU_NS_END