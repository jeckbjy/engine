//! Server
#include "Cute/LogicQueue.h"

CUTE_NS_BEGIN

//static bool event_delay_cmp(LogicEvent* ev1, LogicEvent* ev2)
//{
//	return ev1->getTime() >= ev2->getTime();
//}
//
//static bool event_prio_front_cmp(LogicEvent* ev1, LogicEvent* ev2)
//{
//	return ev1->getPriority() >= ev2->getPriority();
//}
//
//static bool event_prio_back_cmp(LogicEvent* ev1, LogicEvent* ev2)
//{
//	return ev1->getPriority() > ev2->getPriority();
//}

struct EventDelayFinder
{
	LogicEvent* ev1;
	EventDelayFinder(LogicEvent* ev) :ev1(ev){}
	bool operator()(LogicEvent* ev2)
	{
		return ev1->getTime() >= ev2->getTime();
	}
};

struct EventPrioFrontFinder
{
	LogicEvent* ev1;
	EventPrioFrontFinder(LogicEvent* ev):ev1(ev){}
	bool operator()(LogicEvent* ev2)
	{
		return ev1->getPriority() >= ev2->getPriority();
	}
};

struct EventPrioBackFinder
{
	LogicEvent* ev1;
	EventPrioBackFinder(LogicEvent* ev) :ev1(ev){}
	bool operator()(LogicEvent* ev2)
	{
		return ev1->getPriority() > ev2->getPriority();
	}
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LogicQueue::LogicQueue()
{
}

LogicQueue::~LogicQueue()
{
}

void LogicQueue::process()
{
	EventList events;
	pop(events);
	// 遍历处理
	LogicEvent* ev;
	for (EventList::Iterator itor = events.begin(); itor != events.end();)
	{
		ev = *itor;
		itor = events.erase(itor);
		ev->process();
		//ev->destroy();
	}
}

void LogicQueue::push(LogicEvent* ev)
{
	Mutex::ScopedLock guard(m_mutex);
	if (ev->getTime() > 0)
	{
		//EventList::ReverseIterator itor = std::find_if(m_delay.rbegin(), m_delay.rend(), EventDelayFinder(ev));
		//m_delay.insert(itor, ev);
	}
	else if (ev->getPriority() > LogicEvent::PRIO_NORMAL)
	{
		EventList::Iterator itor = std::find_if(m_priority.begin(), m_priority.end(), EventPrioFrontFinder(ev));
		m_priority.insert(itor, ev);
	}
	else
	{
		//EventList::ReverseIterator itor = std::find_if(m_priority.rbegin(), m_priority.rend(), EventPrioBackFinder(ev));
		//m_priority.insert(itor, ev);
	}
}

void LogicQueue::pop(EventList& events)
{
	int64_t timestamp = LogicEvent::now();

	Mutex::ScopedLock guard(m_mutex);
	events.swap(m_priority);
	// 遍历到时间的
	LogicEvent* ev;
	for (EventList::Iterator itor = m_delay.begin(); itor != m_delay.end(); )
	{
		ev = *itor;
		if (itor->getTime() > timestamp)
		{
			break;
		}

		// 删除无效的

		itor = m_delay.erase(itor);
		events.push_back(ev);
	}
}

CUTE_NS_END
