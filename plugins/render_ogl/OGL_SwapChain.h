#include "OGL_API.h"
#include "Window.h"

CU_NS_BEGIN

class CU_OGL_API OGL_SwapChain : public SwapChain
{
public:
	OGL_SwapChain(Window* wnd);
	~OGL_SwapChain();

	void present();

private:
	Window*		m_wnd;
#if defined(CU_OS_WIN)
	HDC			m_hdc;
	HGLRC		m_hrc;
#elif defined(CU_OS_LINUX)
	::Display	m_display;
#endif
};

CU_NS_END