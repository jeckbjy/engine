#include "OGL_RenderWindow.h"

CU_NS_BEGIN

OGLRenderWindow::OGLRenderWindow(Window* wnd)
:OGLFrameBuffer(false)
{
#ifdef CU_OS_WIN
	_wnd = (HWND)wnd->handle();
	_hdc = GetDC(_wnd);
	_hrc = wglCreateContext(_hdc);
	if (!_hrc)
	{
		DWORD error = GetLastError();
		throw std::runtime_error("wglCreateContext error");
	}
	wglMakeCurrent(_hdc, _hrc);
#elif defined(CU_OS_LINUX)
#endif

	// Ïß¿òÄ£Ê½
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

OGLRenderWindow::~OGLRenderWindow()
{
#ifdef CU_OS_WIN
	if (_hrc)
	{
		wglDeleteContext(_hrc);
		_hrc = NULL;
	}
	if (_hdc)
	{
		::ReleaseDC(_wnd, _hdc);
		_hdc = NULL;
	}
#else
#endif
}

void OGLRenderWindow::present()
{
#if defined(CU_OS_WIN)
	::SwapBuffers(_hdc);
#elif defined(CU_OS_LINUX)
	::glXSwapBuffers(_display, _hwnd);
#endif
}

void OGLRenderWindow::bind()
{
	if (bindFBO(0))
	{
		// enable SRGB??
		GLenum targets[] = { GL_BACK_LEFT };
		glDrawBuffers(1, targets);
	}
}

void OGLRenderWindow::attach(uint32_t, Texture*)
{
	throw std::runtime_error("RenderWindow cannot use attach");
}

void OGLRenderWindow::detach(uint32_t)
{
	throw std::runtime_error("RenderWindow cannot use detach");
}

CU_NS_END