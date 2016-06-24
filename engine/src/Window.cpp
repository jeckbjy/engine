#include "Window.h"

CU_NS_BEGIN

#ifdef CU_OS_WINNT

static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG_PTR ptr = NULL;
	Window* wnd = NULL;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		ptr = (LONG_PTR)cs->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, ptr);
	}
	else
	{
		ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (ptr != NULL)
	{
		Window* wnd = (Window*)ptr;
		return wnd->processMsg(uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

static void MSWGetStyle(long flags, DWORD& style, DWORD& styleEx)
{
	style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	styleEx = WS_EX_APPWINDOW;
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
	// ±ß¿ò£¿£¿
}

static const CHAR* MSWGetRegisterClass()
{
	static const CHAR* CLASS_NAME = "WINDOW";
	static bool hasRegister = false;
	if (hasRegister)
		return CLASS_NAME;
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)__WndProc;
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = CLASS_NAME;

	if (!::RegisterClassEx(&wc))
	{
		return NULL;
	}

	hasRegister = true;
	return CLASS_NAME;
}

static HWND MSWCreateWindow(const char* title, size_t width, size_t height, long flags, void* udata)
{
	const CHAR* clsName = MSWGetRegisterClass();
	if (!clsName)
		return 0;
	DWORD style, styleEx;
	MSWGetStyle(flags, style, styleEx);
	HWND hWnd = CreateWindowEx(
		styleEx, clsName, title, style, 
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, ::GetModuleHandle(NULL), udata);
	return hWnd;
}

#endif

#ifdef CU_OS_WINNT
LRESULT Window::processMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		m_closed = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		break;
	case WM_PAINT:
		break;
	case WM_SIZE:
	{
		size_t w = LOWORD(lParam);
		size_t h = HIWORD(lParam);
		break;
	}
	case WM_SIZING:
		break;
	default:
		break;
	}

	return DefWindowProc(m_handle, uMsg, wParam, lParam);
}
#endif

Window::Window()
{
	m_handle = 0;
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
}

Window::Window(Window* parent, const String& title /* = "" */, size_t width, size_t height,long flags /* = 0 */)
{
#if defined(CU_OS_WINNT)
	m_closed = false;
	m_handle = MSWCreateWindow(title.c_str(), width, height, flags, this);
	RECT rect;
	if (GetWindowRect(m_handle, &rect))
	{
		m_x = rect.left;
		m_y = rect.top;
	}

	if (GetClientRect(m_handle, &rect))
	{
		m_width = rect.right - rect.left;
		m_height = rect.bottom - rect.top;
	}

	if (flags & WF_VISIBLE)
		show(true);

#elif defined(CU_OS_ANDROID)
#elif defined(CU_USE_X11)
#elif defined(CU_USE_XCB)
#endif
}

Window::~Window()
{

}

void Window::show(bool showFlag)
{
#ifdef CU_OS_WINNT
	::ShowWindow(m_handle, showFlag ? SW_SHOW : SW_HIDE);
#endif
}

CU_NS_END
