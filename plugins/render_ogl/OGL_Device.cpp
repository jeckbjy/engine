#include "OGL_Device.h"
#include "OGL_Mapping.h"
#include "OGL_Program.h"
#include "OGL_Texture.h"
#include "OGL_Buffer.h"
#include "OGL_RenderWindow.h"
#include "OGL_RenderTexture.h"
#include "OGL_VertexLayout.h"
#include "OGL_CommandBuffer.h"
#include "OGL_Pipeline.h"
#include "OGL_DescriptorSet.h"
#include "Window.h"

CU_NS_BEGIN

#ifdef WIN32
// 初始化glew必须要先创建一个窗体设置wgl
class OGLWindowContext
{
public:
	OGLWindowContext()
		:_free(false), _hwnd(NULL), _hdc(NULL), _hrc(NULL)
	{
		_hrc = wglGetCurrentContext();
		if (_hrc)
			return;
		if (Window::current() == NULL)
		{
			_hwnd = Window::create_window("__gl_init_tmp", 0, 0);
			_free = true;
		}
		else
		{
			_hwnd = Window::current()->handle();
			_free = false;
		}
		if (_hwnd)
		{
			_hdc = GetDC(_hwnd);
			_hrc = wglCreateContext(_hdc);
			if (_hdc && _hrc)
				wglMakeCurrent(_hdc, _hrc);

		}
	}
	~OGLWindowContext()
	{
		if (_free)
		{
			if (_hrc)
				wglDeleteContext(_hrc);
			if (_hdc)
				::ReleaseDC(_hwnd, _hdc);
			if (_hwnd)
				::DestroyWindow(_hwnd);
		}
	}
private:
	bool	_free;
	HWND	_hwnd;
	HDC		_hdc;
	HGLRC	_hrc;
};
#endif

OGLDevice::OGLDevice()
:_name("OGLDevice")
{

}

OGLDevice::~OGLDevice()
{

}

bool OGLDevice::init()
{
#ifdef CU_USE_GLEW
#ifdef WIN32
	OGLWindowContext context;
#endif
	if (GLEW_OK != glewInit())
		return false;
#else
#endif

	return true;
}

GpuBuffer* OGLDevice::newBuffer(const BUFFER_DESC& desc)
{
	return new OGLBuffer(desc);
}

Texture* OGLDevice::newTexture(const TEXTURE_DESC& desc)
{
	return new OGLTexture(desc);
}

RenderTarget* OGLDevice::newRenderWindow(Window* wnd)
{
	return new OGLRenderWindow(wnd);
}

RenderTarget* OGLDevice::newRenderTexture(Texture* color, Texture* depth_stencil)
{
	return new OGLRenderTexture(color, depth_stencil);
}

DescriptorSet* OGLDevice::newDescriptorSet(Program* prog)
{
	return new OGLDescriptorSet(prog);
}

VertexLayout* OGLDevice::newVertexLayout(VertexDeclaration& desc)
{
	return new OGLVertexLayout(desc);
}

Program* OGLDevice::newProgram()
{
	return new OGLProgram();
}

Pipeline* OGLDevice::newPipeline(const PIPELINE_DESC& desc)
{
	return new OGLPipeline(desc);
}

CommandBuffer* OGLDevice::newCommandBuffer()
{
	return new OGLCommandBuffer();
}

CommandQueue* OGLDevice::newCommandQueue()
{
	return new OGLCommandQueue();
}

void OGLDevice::execute(CommandBuffer* cmd)
{
	((OGLCommandBuffer*)cmd)->execute();
}

CU_NS_END