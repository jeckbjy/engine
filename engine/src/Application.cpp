//! Application
#include "Cute/Application.h"
#include "Cute/Files.h"
#include "MessagePump.h"

CUTE_NS_BEGIN

Application* Application::s_instance = 0;

Application::Application()
{
	assert(s_instance == NULL);
	s_instance = this;
}

Application::~Application()
{
	s_instance = NULL;
}

int Application::run()
{
    for(;;)
    {
        MessagePump::dispatch();
    }
//	try
//	{
//		if (!setup())
//			return EXIT_CONFIG;
//		
//        loop();
//		quit();
//	}
//	catch (std::exception& e)
//	{
//		if (m_logger)
//			m_logger->log(LOG_LEVEL_FATAL, "", e.what());
//	}

	return EXIT_SUCCESS;
}

int Application::run(int argc, char *argv[])
{
//	if (argc != 0 && argv != 0)
//		m_options.parse(argc, argv);

	return run();
}

bool Application::setup()
{
//	getPath();
//
//	// load config
//	m_options.get(m_configPath, "config");
//	if (m_configPath.empty())
//		m_configPath = "./" + m_name + ".cfg";
//	if (Files::exists(m_configPath))
//		m_config.load(m_configPath);

	// init logger

	return true;
}

void Application::quit()
{
}

void Application::loop()
{
//    while(1)
//    {
//        MessagePump::dispatch();
//    }
    // 执行消息
}

//void Application::getPath()
//{
//#ifdef _WIN32
//	char szModuleName[MAX_PATH];
//	GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
//	m_path = szModuleName;
//#else
//	char szModuleName[MAX_PATH] = { 0 };
//	readlink("/proc/self/exe", szModuleName, MAX_PATH);
//	m_path = szModuleName;
//#endif
//	size_t sindex = m_path.find_last_of(CUTE_PATH_SEP);
//	size_t eindex = m_path.find_last_of('.');
//	if (sindex != String::npos && eindex != String::npos)
//		m_name = m_path.substr(sindex + 1, eindex - sindex);
//}

CUTE_NS_END
