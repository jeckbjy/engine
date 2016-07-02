#include "OGL_SwapChain.h"
#include "Window.h"

CU_NS_BEGIN

OGL_SwapChain::OGL_SwapChain(const SwapChainDesc& desc)
	: m_wnd(desc.wnd)
{
#if defined(CU_OS_WIN)
	HWND hwnd = m_wnd->getHandle();

	// create hdc and set format
	m_hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	// todo: PFD_STEREO
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = PixelUtil::getColorBits(desc.colorFormat);
	pfd.cDepthBits = PixelUtil::getDepthBits(desc.depthStencilFormat);
	pfd.cStencilBits = PixelUtil::getStencilBits(desc.depthStencilFormat);

	int pf = ::ChoosePixelFormat(m_hdc, &pfd);
	::SetPixelFormat(m_hdc, pf, &pfd);

	// create context
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

void OGL_SwapChain::bind(void*)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLenum targets[] = { GL_BACK_LEFT };
	glDrawBuffers(1, &targets[0]);
}

CU_NS_END