#pragma once
#include "OGL_API.h"
#include "OGL_FrameBuffer.h"
#include "Window.h"

CU_NS_BEGIN

// glReadBuffer,glReadPixels���ڽ�ͼ
// glDrawBuffers,glDrawBuffer:����shaderҪд�Ļ�����
// GL_FRONT,GL_BACK,GL_BACK_LEFT��
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
	HWND			_wnd;	// �������ͷ�
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