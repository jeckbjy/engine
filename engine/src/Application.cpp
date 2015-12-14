#include "Application.h"
#include "Engine.h"

CU_NS_BEGIN

Application::Application()
{

}

Application::~Application()
{

}

int Application::run()
{
	try
	{
		init();

	}
	catch (std::exception exp)
	{
		release();
		return EXIT_FAILURE;
	}
	return 0;
}

CU_NS_END