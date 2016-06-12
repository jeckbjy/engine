#include "OGL_Device.h"
#include "OGL_Mapping.h"
#include "OGL_Program.h"
#include "OGL_Texture.h"
#include "OGL_Buffer.h"
#include "OGL_InputLayout.h"
#include "OGL_CommandBuffer.h"
#include "OGL_Pipeline.h"
#include "OGL_DescriptorSet.h"
#include "OGL_FrameBuffer.h"
#include "OGL_SwapChain.h"
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
		/*_hrc = wglGetCurrentContext();
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

		}*/
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

OGL_Device::OGL_Device()
{

}

OGL_Device::~OGL_Device()
{

}

bool OGL_Device::init()
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

GpuBuffer* OGL_Device::newBuffer(const BufferDesc& desc)
{
	return new OGL_Buffer(desc);
}

Texture* OGL_Device::newTexture(const TextureDesc& desc)
{
	return new OGL_Texture(desc);
}

DescriptorSet* OGL_Device::newDescriptorSet(Pipeline* pipeline)
{
	return new OGL_DescriptorSet(pipeline);
}

InputLayout* OGL_Device::newInputLayout(const InputElement* elements, size_t count)
{
	return new OGL_InputLayout(elements, count);
}

ShaderStage* OGL_Device::newShader()
{
	return new OGL_Shader();
}

ShaderProgram* OGL_Device::newProgram()
{
	return new OGL_Program();
}

Pipeline* OGL_Device::newPipeline(const PipelineDesc& desc)
{
	return new OGL_Pipeline(desc);
}

CommandBuffer* OGL_Device::newCommandBuffer()
{
	return new OGL_CommandBuffer();
}

CommandQueue* OGL_Device::newCommandQueue()
{
	return new OGLCommandQueue();
}

FrameBuffer* OGL_Device::newFrameBuffer()
{
	return new OGL_FrameBuffer();
}

SwapChain* OGL_Device::newSwapChain(Window* wnd)
{
	return new OGL_SwapChain(wnd);
}

CU_NS_END
