#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

/// This is the base class for thread error handlers.
///
/// An unhandled exception that causes a thread to terminate is usually
/// silently ignored, since the class library cannot do anything meaningful
/// about it.
/// 
/// The Thread class provides the possibility to register a
/// global ErrorHandler that is invoked whenever a thread has
/// been terminated by an unhandled exception.
/// The ErrorHandler must be derived from this class and can
/// provide implementations of all three exception() overloads.
///
/// The ErrorHandler is always invoked within the context of
/// the offending thread.
class CUTE_CORE_API ErrorHandler
{
public:
	ErrorHandler();
	virtual ~ErrorHandler();


	virtual void exception(const Exception& exc);
	virtual void exception(const std::exception& exc);
	virtual void exception();

	static void handle(const Exception& exc);
	static void handle(const std::exception& exc);
	static void handle();
	static ErrorHandler* set(ErrorHandler* pHandler);
	static ErrorHandler* get();

protected:
	static ErrorHandler* defaultHandler();

private:
	static ErrorHandler* m_handler;
	static Mutex		m_mutex;
};

//
// inlines
//
inline ErrorHandler* ErrorHandler::get()
{
	return m_handler;
}

CUTE_NS_END