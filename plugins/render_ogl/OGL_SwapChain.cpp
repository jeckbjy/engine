#include "OGL_SwapChain.h"

CU_NS_BEGIN

OGL_SwapChain::OGL_SwapChain(Window* wnd)
	: m_wnd(wnd)
{
#if defined(CU_OS_WIN)
	HWND hwnd = m_wnd->getHandle();
	m_hdc = GetDC(hwnd);
	m_hrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, m_hrc);
#elif defined(CU_USE_X11)
#elif defined(CU_USE_XCB)
#endif
}

OGL_SwapChain::~OGL_SwapChain()
{
#if defined(CU_OS_WIN)
	if (m_hrc)
		wglDeleteContext(m_hrc);

	if (m_hdc)
		::ReleaseDC(m_wnd->getHandle(), m_hdc);

#elif defined(CU_USE_X11)
#elif defined(CU_USE_XCB)
#endif
}

void OGL_SwapChain::present()
{
#ifdef CU_OS_WIN
	::SwapBuffers(m_hdc);
#else
	//::glXSwapBuffers();
#endif
}

CU_NS_END