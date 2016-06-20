#include "Window.h"

CU_NS_BEGIN

static uint32 __WIN_MAXID = 0;

#ifdef CU_OS_WINNT

static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);
		return 0;
	}
	LONG_PTR ptr = GetWindowLongPtr(hWnd, 0);
	if (ptr == NULL)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	Window* wnd = (Window*)ptr;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		//wnd->onClose();
	}
	break;
	case WM_ACTIVATE:
	{
		BOOL focused = LOWORD(wParam) != WA_INACTIVE;
		BOOL iconified = HIWORD(wParam) ? TRUE : FALSE;
		if (focused && iconified)
			focused = FALSE;
		//wnd->onActive(focused == TRUE);
	}
	break;
	case WM_PAINT:
	{
		//wnd->onPaint();
		break;
	}
	case WM_SIZE:
	{
		size_t w = LOWORD(lParam);
		size_t h = HIWORD(lParam);
		//wnd->onResize(w, h);
		break;
	}
	case WM_SIZING:
	{
		POINT topLeftClient = { 0, 0 };
		RECT rect;
		GetWindowRect(wnd->handle(), &rect);
		ClientToScreen(wnd->handle(), &topLeftClient);
		OffsetRect(&rect, topLeftClient.x, topLeftClient.y);
		//wnd->onResize(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

window_t create_window(const WINDOW_DESC& desc)
{
	if (desc.hwnd != INVALID_HANDLE_VALUE)
	{
		return desc.hwnd;
	}

	INT32 left = desc.left;
	INT32 top = desc.top;
	UINT32 width = desc.width;
	UINT32 height = desc.height;

	bool isChild = (desc.parent != INVALID_HANDLE_VALUE);
	bool isFullscreen = desc.isFullscreen && !isChild;
	// Check Style And StyleEx
	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD styleEx = WS_EX_APPWINDOW;
	if (isFullscreen)
	{
		style |= WS_POPUP;
	}
	else
	{
		// toolwindow
		if (desc.isToolWindow)
			styleEx = WS_EX_TOOLWINDOW;

		// child
		if (isChild)
			style |= WS_CHILD;

		// border
		if (desc.border == WIN_BORDER_NONE)
			style |= WS_POPUP;
		else if (desc.border == WIN_BORDER_FIXED)
			style |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		else
			style |= WS_OVERLAPPEDWINDOW;
	}

	// check monitor and set to center when left==-1
	if (!isFullscreen)
	{
		HMONITOR monitor = NULL;
		if (monitor == NULL)
		{
			POINT point = { left, top };
			monitor = MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY);
		}

		// Get the target monitor info
		MONITORINFO monitorInfo;
		memset(&monitorInfo, 0, sizeof(MONITORINFO));
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitorInfo);

		if (left == -1 || top == -1)
		{
			int screenw = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
			int screenh = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;

			// clamp window dimensions to screen size
			int outerw = (int(width) < screenw) ? int(width) : screenw;
			int outerh = (int(height) < screenh) ? int(height) : screenh;

			if (left == -1)
				left = monitorInfo.rcWork.left + (screenw - outerw) / 2;
			else if (monitor != NULL)
				left += monitorInfo.rcWork.left;

			if (top == -1)
				top = monitorInfo.rcWork.top + (screenh - outerh) / 2;
			else if (monitor != NULL)
				top += monitorInfo.rcWork.top;
		}
		else if (monitor != NULL)
		{
			left += monitorInfo.rcWork.left;
			top += monitorInfo.rcWork.top;
		}

		if (!desc.isOuterDimensions)
		{
			// Calculate window dimensions required to get the requested client area
			RECT rect;
			SetRect(&rect, 0, 0, width, height);
			AdjustWindowRect(&rect, style, false);
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;

			// Clamp width and height to the desktop dimensions
			int screenw = GetSystemMetrics(SM_CXSCREEN);
			int screenh = GetSystemMetrics(SM_CYSCREEN);

			if ((int)width > screenw)
				width = screenw;

			if ((int)height > screenh)
				height = screenh;

			if (left < 0)
				left = (screenw - width) / 2;

			if (top < 0)
				top = (screenh - height) / 2;
		}
	}

	HINSTANCE module;
#ifdef CU_BUILD_LIB
	module = GetModuleHandle(NULL);
#else
	module = GetModuleHandle("engine.dll");
#endif
	if (module == NULL)
		module = GetModuleHandle(NULL);

	// Create Window
	const CHAR* class_name = "CLASS_WIN32WND";
	UINT classStyle = 0;
	if (desc.isDoubleClick)
		classStyle |= CS_DBLCLKS;
	WNDCLASS wc = {
		classStyle, __WndProc, 0, 0, module,
		LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH), 0, class_name
	};

	if (!::RegisterClass(&wc))
	{
		throw std::runtime_error("RegisterClassEx error");
		return NULL;
	}

	window_t hwnd = CreateWindowEx(style, class_name, desc.title.c_str(), style,
		left, top, width, height, desc.parent, NULL, module, desc.udata);

	if (hwnd == NULL)
	{
		throw std::runtime_error("CreateWindowEx error");
		return NULL;
	}
	// 设置像素格式
	HDC hdc = GetDC(hwnd);
	if (hdc != NULL)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 15;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		int format = ChoosePixelFormat(hdc, &pfd);
		if (format != 0)
			::SetPixelFormat(hdc, format, &pfd);
		::ReleaseDC(hwnd, hdc);
	}
	return hwnd;
}

bool destroy_window(window_t hwnd)
{
	return ::DestroyWindow(hwnd) == TRUE;
}

#endif

#ifdef CU_OS_WIN
bool Window::pollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return true;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return false;
}
#else
bool Window::pollEvents()
{
	return false;
}
#endif

Window::Window(const WINDOW_DESC& desc)
{
	m_hwnd = create_window(desc);
	m_id = ++__WIN_MAXID;
	m_udata = desc.udata;
	m_flags = 0;
	m_multisample = desc.multisample;
	m_vsync = desc.vsync;
	m_refreshRate = desc.refreshRate;
	setFlag(WF_FULLSCREEN, desc.isFullscreen);
	setFlag(WF_HIDDEN, desc.isHidden);
	setFlag(WF_DEPTH_STENCIL, desc.isDepthStencil);
	setFlag(WF_GAMMA, desc.isGamma);
}

Window::~Window()
{
	if ((m_hwnd != NULL) && (m_flags & WF_AUTO_FREE))
	{
		destroy_window(m_hwnd);
		m_hwnd = NULL;
	}
}

void Window::resize(size_t width, size_t height)
{
#ifdef CU_OS_WINNT
	if (m_hwnd)
	{
		RECT rc = { 0, 0, (LONG)width, (LONG)height };
		AdjustWindowRect(&rc, GetWindowLong(m_hwnd, GWL_STYLE), false);
		m_width = rc.right - rc.left;
		m_height = rc.bottom - rc.top;

		SetWindowPos(m_hwnd, HWND_TOP, m_left, m_top, m_width, m_height, SWP_NOMOVE);
	}
#endif
}

void Window::move(int32_t x, int32_t y)
{
#ifdef CU_OS_WINNT
	if (m_hwnd)
	{
		m_left = x;
		m_top = y;
		SetWindowPos(m_hwnd, HWND_TOP, m_left, m_top, m_width, m_height, SWP_NOSIZE);
	}
#endif
}

void Window::minimize()
{
#ifdef CU_OS_WINNT
	if (m_hwnd)
		ShowWindow(m_hwnd, WS_MINIMIZE);
#endif
}

void Window::maximize()
{
#ifdef CU_OS_WINNT
	if (m_hwnd)
		ShowWindow(m_hwnd, SW_MAXIMIZE);
#endif
}

void Window::restore()
{
#ifdef CU_OS_WINNT
	if (m_hwnd)
		ShowWindow(m_hwnd, SW_RESTORE);
#endif
}

void Window::setFlag(WindowFlag mask, bool flag)
{
	if (flag)
		m_flags |= mask;
	else
		m_flags &= ~mask;
}

void Window::setHidden(bool hidden)
{
#ifdef CU_OS_WINNT
	if (hidden)
		ShowWindow(m_hwnd, SW_HIDE);
	else
		ShowWindow(m_hwnd, SW_SHOW);
#endif

	// 修改状态标识
	setFlag(WF_HIDDEN, hidden);
}

void Window::setActive(bool state)
{
#ifdef CU_OS_WINNT
	if (state)
		ShowWindow(m_hwnd, SW_RESTORE);
	else
		ShowWindow(m_hwnd, SW_SHOWMINNOACTIVE);
#endif
	setFlag(WF_ACTIVE, state);
}

void Window::show()
{
	setHidden(false);
}

void Window::hide()
{
	setHidden(true);
}

CU_NS_END
