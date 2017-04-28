#include "OGL_API.h"

CUTE_NS_BEGIN

class CU_OGL_API OGL_SwapChain : public SwapChain
{
public:
	OGL_SwapChain(const SwapChainDesc& desc);
	~OGL_SwapChain();

	void present();
	void bind(void*);

private:
	WindowPtr	m_wnd;
#if defined(CU_OS_WIN)
	HDC			m_hdc;
	HGLRC		m_hrc;
#elif defined(CU_OS_LINUX)
	::Display	m_display;
#endif
};

CUTE_NS_END
