#include "Application.h"
#include "Engine.h"

CU_NS_BEGIN

Application::Application()
{

}

Application::~Application()
{

}

void Application::run()
{
	try
	{
		if (!init() || !gEngine.init())
			return;
		while (!m_quit)
		{
			gEngine.update();
			update();
			pumpMsg();
		}
	}
	catch (std::exception exp)
	{
		release();
	}
}

void Application::pumpMsg()
{
#ifdef CU_OS_WINNT
	MSG msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#else

#endif
}

CU_NS_END
