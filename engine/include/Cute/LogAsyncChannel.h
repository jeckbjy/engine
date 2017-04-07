#pragma once
#include "Cute/LogChannel.h"
#include "Cute/Event.h"
#include "Cute/Mutex.h"
#include "Cute/Thread.h"
#include "Cute/Runnable.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

class CUTE_CORE_API LogAsyncChannel : public LogChannel, public Runnable
{
public:
	LogAsyncChannel();
	~LogAsyncChannel();

	void addChannel(LogChannel* channel);

	void open();
	void close();

	void log(const LogMessage& msg);
	void run();

private:
	typedef RefPtr<LogChannel>		ChannelPtr;
	typedef std::vector<ChannelPtr> ChannelVec;
	typedef std::queue<LogMessage>	MessageQueue;
	ChannelVec		m_channels;
	MessageQueue	m_messages;
	Thread			m_thread;
	Mutex			m_mutex;
	Event			m_event;
	bool			m_opened;
};

CUTE_NS_END