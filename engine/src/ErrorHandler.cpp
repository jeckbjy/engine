// module Thread
#include "Cute/ErrorHandler.h"

CUTE_NS_BEGIN

ErrorHandler* ErrorHandler::m_handler = ErrorHandler::defaultHandler();
Mutex ErrorHandler::m_mutex;

ErrorHandler::ErrorHandler()
{
}

ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::exception(const Exception& exc)
{
	cute_debugger_msg(exc.what());
}

void ErrorHandler::exception(const std::exception& exc)
{
	cute_debugger_msg(exc.what());
}

void ErrorHandler::exception()
{
	cute_debugger_msg("unknown exception");
}

void ErrorHandler::handle(const Exception& exc)
{
	Mutex::ScopedLock lock(m_mutex);
	try
	{
		m_handler->exception(exc);
	}
	catch (...)
	{
	}
}

void ErrorHandler::handle(const std::exception& exc)
{
	Mutex::ScopedLock lock(m_mutex);
	try
	{
		m_handler->exception(exc);
	}
	catch (...)
	{
	}
}

void ErrorHandler::handle()
{
	Mutex::ScopedLock lock(m_mutex);
	try
	{
		m_handler->exception();
	}
	catch (...)
	{
	}
}

ErrorHandler* ErrorHandler::set(ErrorHandler* pHandler)
{
	cute_check_ptr(pHandler);

	Mutex::ScopedLock lock(m_mutex);
	ErrorHandler* pOld = m_handler;
	m_handler = pHandler;
	return pOld;
}

ErrorHandler* ErrorHandler::defaultHandler()
{
	// NOTE: Since this is called to initialize the static _pHandler
	// variable, sh has to be a local static, otherwise we run
	// into static initialization order issues.
	static ErrorHandler sh;
	return &sh;
}

CUTE_NS_END