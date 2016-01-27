#include "Window.h"

CU_NS_BEGIN

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
		wnd->onClose();
	}
	break;
	case WM_ACTIVATE:
	{
		BOOL focused = LOWORD(wParam) != WA_INACTIVE;
		BOOL iconified = HIWORD(wParam) ? TRUE : FALSE;
		if (focused && iconified)
			focused = FALSE;
		wnd->onActive(focused == TRUE);
	}
	break;
	case WM_PAINT:
	{
		wnd->onPaint();
		break;
	}
	case WM_SIZE:
	{
		size_t w = LOWORD(lParam);
		size_t h = HIWORD(lParam);
		wnd->onResize(w, h);
		break;
	}
	case WM_SIZING:
	{
		POINT topLeftClient = { 0, 0 };
		RECT rect;
		GetWindowRect(wnd->handle(), &rect);
		ClientToScreen(wnd->handle(), &topLeftClient);
		OffsetRect(&rect, topLeftClient.x, topLeftClient.y);
		wnd->onResize(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

const CHAR* __CLASS_NAME = "CLS_CUTE";

static void registerClass()
{
	// 标识是否已经注册
	static bool has_register = false;
	if (has_register)
		return;
	// 注册窗体类
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)(__WndProc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(void*) + sizeof(int);// Make room for one pointer
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;  // No brush - we are going to paint our own background
	wc.lpszMenuName = NULL;  // No default menu
	wc.lpszClassName = __CLASS_NAME;

	if (!::RegisterClassEx(&wc))
	{
		throw std::runtime_error("RegisterClassEx error");
		return;
	}

	has_register = true;
}

window_t Window::create_window(const char* name, size_t width, size_t height, int flags /* = 0 */, void* udata /* = NULL */)
{
	registerClass();
	// 初始化参数
	int xpos = CW_USEDEFAULT;
	int ypos = CW_USEDEFAULT;

	// 创建窗体
	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD styleEx = WS_EX_APPWINDOW;
	if (flags & WF_FULLSCREEN)
	{
		style |= WS_POPUP;
	}
	else
	{
		if (flags & WF_RESIZABLE)
			style |= WS_OVERLAPPEDWINDOW;
		else
			style |= WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;
		styleEx |= WS_EX_WINDOWEDGE;
	}
	HWND hwnd = CreateWindowEx(styleEx, __CLASS_NAME, name, style,
		xpos, ypos, width, height,
		NULL, NULL, ::GetModuleHandle(NULL), udata);
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

	// 设置是否可见
	if (flags & WF_VISIBLE)
		::ShowWindow(hwnd, SW_SHOW);
	return hwnd;
}

bool Window::destroy_window(window_t hwnd)
{
	return ::DestroyWindow(hwnd) == TRUE;
}

//bool Window::pollEvents()
//{
//	MSG msg;
//	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//	{
//		if (msg.message == WM_QUIT)
//			return true;
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//	return false;
//}

#endif

Window* g_cur_wnd = NULL;
Window* Window::current() { return g_cur_wnd; }

Window::Window(window_t hwnd)
	:m_hwnd(hwnd), m_flags(0), m_width(0), m_height(0), m_udata(NULL)
{
	if (m_hwnd != NULL)
		g_cur_wnd = this;
}

Window::~Window()
{
	if ((m_flags & WF_AUTO_FREE) && m_hwnd)
	{
		destroy_window(m_hwnd);
	}
	if (g_cur_wnd == this)
		g_cur_wnd = NULL;
}

bool Window::create(const char* name, size_t width /* = 1024 */, size_t height /* = 768 */, int flags /* = 0 */)
{
	m_hwnd = create_window(name, width, height, flags, this);
	if (m_hwnd == NULL)
		return false;
	m_width = width;
	m_height = height;
	m_flags = flags;
	m_flags |= WF_AUTO_FREE;
	g_cur_wnd = this;
	return true;
}

void Window::resize(size_t w, size_t h)
{
	m_width = w;
	m_height = h;
}

void Window::show()
{
	if (m_flags & WF_VISIBLE)
		return;
	m_flags |= WF_VISIBLE;
#ifdef CU_OS_WINNT
	::ShowWindow(m_hwnd, SW_SHOW);
#endif
}

void Window::hide()
{
	if (m_flags & WF_VISIBLE)
	{
		CU_CLEAR_MASK(m_flags, WF_VISIBLE);
#ifdef CU_OS_WINNT
		::ShowWindow(m_hwnd, SW_HIDE);
#endif
	}
}

void Window::onClose()
{
	m_flags |= WF_CLOSED;
}

void Window::onActive(bool flag)
{

}

void Window::onResize(size_t width, size_t height)
{
	m_width = width;
	m_height = height;
}

CU_NS_END