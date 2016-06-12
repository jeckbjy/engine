#include "OGL_API.h"

CU_NS_BEGIN

class CU_OGL_API OGL_SwapChain : public SwapChain
{
public:
	OGL_SwapChain(Window* wnd);
	~OGL_SwapChain();

	void present();

private:
	Window*	m_wnd;
};

CU_NS_END