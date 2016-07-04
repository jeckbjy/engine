#include "OGL_Plugin.h"
#include "OGL_Device.h"
#include "OGL_SwapChain.h"

CU_NS_BEGIN

DEF_PLUGIN(CU_OGL_API, OGL_Plugin)

OGL_Plugin::OGL_Plugin()
	: Plugin(OGL_NAME)
	, m_device(NULL)
{

}

OGL_Plugin::~OGL_Plugin()
{
	CU_SAFE_RELEASE(m_device);
}

void OGL_Plugin::install()
{
	if (!m_device)
	{
#ifdef CU_USE_GLEW
		Window* wnd = new Window(NULL, "OGLCreator");
		SwapChainDesc desc(wnd);
		SwapChain* chain = new OGL_SwapChain(desc);
		GLenum result = glewInit();
		delete chain;
		if (result != GLEW_OK)
			throw std::runtime_error("glew init fail!");
#endif
		m_device = new OGL_Device();
		m_device->retain();
	}

	gGraphics.setDevice(m_device);
}

void OGL_Plugin::uninstall()
{
	if ((OGL_Device*)gGraphics.getDevice() == m_device)
	{
		gGraphics.setDevice(NULL);
	}
}

CU_NS_END