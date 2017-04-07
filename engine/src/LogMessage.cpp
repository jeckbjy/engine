//! Logging
#include "Cute/LogMessage.h"
#include "Cute/Exception.h"
#include "Cute/Process.h"
#include "Cute/Thread.h"
#include <algorithm>

CUTE_NS_BEGIN

LogMessage::LogMessage()
	: m_level(LOG_LEVEL_FATAL)
	, m_tid(0)
	, m_pid(0)
	, m_file(0)
	, m_line(0)
{
	init();
}

LogMessage::LogMessage(const String& source, const String& text, LogLevel level)
	: m_source(source)
	, m_text(text)
	, m_level(level)
	, m_tid(0)
	, m_pid(0)
	, m_file(0)
	, m_line(0)
{
	init();
}

LogMessage::LogMessage(const String& source, const String& text, LogLevel level, const char* file, int line)
	: m_source(source)
	, m_text(text)
	, m_level(level)
	, m_tid(0)
	, m_pid(0)
	, m_file(file)
	, m_line(line)
{
	init();
}

LogMessage::LogMessage(const LogMessage& other)
	: m_source(other.m_source)
	, m_text(other.m_text)
	, m_thread(other.m_thread)
	, m_level(other.m_level)
	, m_time(other.m_time)
	, m_tid(other.m_tid)
	, m_pid(other.m_pid)
	, m_file(other.m_file)
	, m_line(other.m_line)
{

}

LogMessage::~LogMessage()
{
}

LogMessage& LogMessage::operator =(const LogMessage& msg)
{
	if (&msg != this)
	{
		LogMessage tmp(msg);
		swap(tmp);
	}

	return *this;
}

void LogMessage::swap(LogMessage& msg)
{
	using std::swap;
	swap(m_source, msg.m_source);
	swap(m_text, msg.m_text);
	swap(m_level,msg.m_level);
	swap(m_time, msg.m_time);
	swap(m_tid,  msg.m_tid);
	swap(m_thread, msg.m_thread);
	swap(m_pid,	 msg.m_pid);
	swap(m_file, msg.m_file);
	swap(m_line, msg.m_line);
}

void LogMessage::init()
{
	m_pid = Process::id();
	Thread* pThread = Thread::current();
	if (pThread)
	{
		m_tid = pThread->id();
		m_thread = pThread->name();
	}
}

CUTE_NS_END