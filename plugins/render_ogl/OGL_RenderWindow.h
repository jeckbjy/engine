#pragma once
#include "OGL_API.h"
#include "OGL_FrameBuffer.h"
#include "Window.h"

CU_NS_BEGIN

// glReadBuffer,glReadPixels用于截图
// glDrawBuffers,glDrawBuffer:设置shader要写的缓冲区
// GL_FRONT,GL_BACK,GL_BACK_LEFT等
class CU_OGL_API OGLRenderWindow : public OGLFrameBuffer
{
public:
	OGLRenderWindow(Window* wnd);
	~OGLRenderWindow();
	void present();
	void bind();
	void attach(uint32_t att, Texture* tex);
	void detach(uint32_t att);

protected:
#if defined(CU_OS_WIN)
	HWND			_wnd;	// 不负责释放
	HDC				_hdc;
	HGLRC			_hrc;
#elif defined(CU_OS_LINUX)
	::Display*		_display;
	::Window		_wnd;
	::GLXContext	_context;
	::GLXFBConfig*	_fbc;
#endif
};

CU_NS_END