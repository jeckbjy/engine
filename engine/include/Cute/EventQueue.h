#pragma once
#include "Cute/EventBase.h"
#include "Cute/Thread.h"
#include "Cute/Mutex.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

class CUTE_CORE_API EventQueue
{
public:
	typedef List<EventBase, &EventBase::m_hook> EventList;
	EventQueue();
	~EventQueue();

	void process();

	void push(EventBase* ev);
	void pop(EventList& events);

private:
	Mutex		m_mutex;
	EventList	m_priority;		// �������ȼ���
	EventList	m_delay;		// ��Ҫ�ӳٴ������Ϣ
};

CUTE_NS_END
