//! Logging
#include "Cute/LogAsyncChannel.h"

CUTE_NS_BEGIN

LogAsyncChannel::LogAsyncChannel()
	: m_opened(false)
{

}

LogAsyncChannel::~LogAsyncChannel()
{
	close();
}

void LogAsyncChannel::addChannel(LogChannel* channel)
{
	Mutex::ScopedLock lock(m_mutex);
	m_channels.push_back(channel);
}

void LogAsyncChannel::open()
{
	Mutex::ScopedLock lock(m_mutex);
	if (!m_opened)
	{
		m_opened = true;
		m_thread.start(*this);
	}
}

void LogAsyncChannel::close()
{
	m_mutex.lock();
	bool opened = m_opened;
	if (m_opened)
		m_opened = false;
	m_mutex.unlock();

	if (opened)
	{
		m_event.notify();
		m_thread.join();
	}
}

void LogAsyncChannel::log(const LogMessage& msg)
{
	open();
	m_mutex.lock();
	m_messages.push(msg);
	m_mutex.unlock();

	m_event.notify();
}

void LogAsyncChannel::run()
{
	while (m_opened)
	{
		m_event.wait();
		m_mutex.lock();
		while (!m_messages.empty())
		{
			MessageQueue msgs;
			msgs.swap(m_messages);
			m_mutex.unlock();

			while (!msgs.empty())
			{
				LogMessage& msg = msgs.front();
				for (ChannelVec::iterator itor = m_channels.begin(); itor != m_channels.end(); ++itor)
				{
					(*itor)->log(msg);
				}

				msgs.pop();
			}

			m_mutex.lock();
		}

		m_mutex.unlock();
	}
}

CUTE_NS_END
