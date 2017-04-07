#pragma once
#include "Cute/LogicEvent.h"
#include "Cute/Thread.h"
#include "Cute/Mutex.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

class CUTE_CORE_API LogicQueue
{
public:
	typedef List<LogicEvent, &LogicEvent::m_hook> EventList;
	LogicQueue();
	~LogicQueue();

	void process();

	void push(LogicEvent* ev);
	void pop(EventList& events);

private:
	Mutex		m_mutex;
	EventList	m_priority;		// �������ȼ���
	EventList	m_delay;		// ��Ҫ�ӳٴ������Ϣ
};

CUTE_NS_END
