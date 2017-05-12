#include "Cute/Window.h"

CUTE_NS_BEGIN

Window::Window(window_t hwnd)
: m_handle(hwnd)
{
	m_handle = hwnd;
	//fillRect();
}

Window::Window(Window* parent, const String& title /* = "" */, size_t width, size_t height, long flags /* = 0 */)
	: m_handle((window_t)INVALID_HANDLE_VALUE)
	, m_flags(flags)
{
	//create(title.c_str(), width, height, flags);

	if (flags & WF_VISIBLE)
		show(true);
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

CUTE_NS_END
