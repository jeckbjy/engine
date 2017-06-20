#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector2.h"

#if defined(CUTE_OS_WINDOWS_NT)
#	include <Windows.h>
#elif defined(CUTE_OS_WINDOWS_RT)
#	include <agile.h>
#elif defined(CUTE_OS_ANDROID)
#	include <android/native_window.h>
#elif defined(CUTE_USE_X11)
#	include <X11/X.h>
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <X11/keysym.h>
#elif defined(CUTE_USE_XCB)
#	include <xcb/xcb.h>
#endif

CUTE_NS_BEGIN

// 定义不同类型窗体指针
#if defined(CUTE_OS_WINDOWS_NT)
typedef HWND window_t;
#elif defined(CUTE_OS_WINDOWS_RT)
typedef Platform::Agile<Windows::UI::Core::CoreWindow> window_t;
#elif defined(CU_OS_ANDROID)
typedef ::ANativeWindow*	window_t;
#elif defined(CUTE_USE_X11)
typedef ::Window			window_t;
typedef ::Display			display_t;
#elif defined(CUTE_USE_XCB)
typedef ::xcb_window_t		window_t;
typedef ::xcb_connection_t* connection_t;
#endif

typedef Vector2I Point;
typedef Vector2I Size;

enum WindowFlag
{
	WF_VISIBLE = 0x01,		// 是否可见
	WF_RESIZABLE = 0x02,		// 是否可改变大小
	WF_FULLSCREEN = 0x04,		// 是否全屏
	WF_CAPTION = 0x08,		// 是否可见标题
	WF_FREE = 0x10,		// 是否需要释放

	WF_DEFAULT = WF_RESIZABLE,
};

class CUTE_CORE_API Window : public Object
{
	DECLARE_RTTI(Window, Object, OID_ANY);
public:
	Window(window_t hwnd);
	Window(Window* parent, const String& title = "", size_t width = 1024, size_t height = 768, long style = WF_DEFAULT);
	virtual ~Window();

	void show(bool showFlag = true);
	//void setTitle(const String& title);
	//String getTitle() const;

	int getX() const { return m_x; }
	int getY() const { return m_y; }
	size_t getWidth() const { return m_width; }
	size_t getHeight() const { return m_height; }

	bool isFullscreen() const { return hasFlag(WF_FULLSCREEN); }

	bool hasFlag(uint32_t mask) const { return (m_flags & mask) == mask; }
	void setFlag(uint32_t mask) { m_flags |= mask; }

	window_t getHandle() { return m_handle; }


#ifdef CUTE_OS_WINNT
	LRESULT processMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

private:
	//void create(const char* title, size_t width, size_t height, long flags);
	//void fillRect();

protected:
	window_t	m_handle;
	uint32_t	m_flags;
	int			m_x;
	int			m_y;
	size_t		m_width;
	size_t		m_height;
};

CUTE_NS_END
