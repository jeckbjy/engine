#include "Application.h"
#include "Engine.h"

CU_NS_BEGIN

Application::Application()
	: m_quit(false)
{

}

Application::~Application()
{

}

void Application::run()
{
	try
	{
		if (!setup() || !gEngine.init())
			return;
		while (!m_quit)
		{
			//gEngine.update();
			update();
			if (dispatchMsg())
			{
				m_quit = true;
				break;
			}
		}
	}
	catch (std::exception e)
	{
	}

	cleanup();
}

bool Application::dispatchMsg()
{
#ifdef CU_OS_WINNT
	MSG msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return true;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return false;
#else

#endif
}

CU_NS_END
