#include "Log.h"
#include "Util.h"

CU_NS_BEGIN

Log gLog;

void Log::LogThread(void* param)
{
	Log* log = (Log*)param;
	log->loop();
}

Log::Log()
	:m_stopped(true)
	, m_level(LL_LOG_MAX)
{
	m_channels.push_back(new FileChannel());
	m_channels.push_back(new ConsoleChannel());
}

Log::~Log()
{
	stop();
	for (auto itor = m_channels.begin(); itor != m_channels.end(); ++itor)
	{
		delete (*itor);
	}
	m_channels.clear();
}

void Log::run()
{
	if (!m_stopped)
		return;
	m_stopped = false;
	m_thread.start(&LogThread, this);
}

void Log::stop()
{
	if (m_stopped)
		return;
	m_stopped = true;
	m_event.notify();
	m_thread.join();
}

void Log::loop()
{
	while (!m_stopped)
	{
		m_event.wait();

		m_mutex.lock();
		MessageQueue msgQueue;
		std::swap(m_messages, msgQueue);
		m_mutex.unlock();

		while (!msgQueue.empty())
		{
			LogMessage& msg = msgQueue.front();
			for (auto itor = m_channels.begin(); itor != m_channels.end(); ++itor)
			{
				(*itor)->write(this, msg);
			}
			msgQueue.pop();
		}
	}
}

void Log::log(const LogMessage& msg)
{
	if (msg.level > m_level || msg.text.empty())
		return;
	m_mutex.lock();
	m_messages.push(msg);
	m_mutex.unlock();

	//m_event.notify();
}

void Log::log(const string& text, LogLevel ll, const char* file /* = 0 */, int line /* = 0 */)
{
	LogMessage msg(ll, file, line);
	msg.text = text;
	log(msg);
}

void Log::log(LogLevel ll, const char* file, int line, const char* fmt, ...)
{
	LogMessage msg(ll, file, line);
	va_list va;
	va_start(va, fmt);
	Util::vformat(msg.text, fmt, va);
	va_end(va);
	log(msg);
}

void Log::setProperty(int type, const std::string& key, const std::string& value)
{
	for (auto itor = m_channels.begin(); itor != m_channels.end(); ++itor)
	{
		LogChannel* channel = *itor;
		if (channel->type() == type)
		{
			channel->setProperty(key, value);
			break;
		}
	}
}

CU_NS_END