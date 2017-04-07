// module Thread
#include "Cute/Condition.h"

CUTE_NS_BEGIN

Condition::Condition()
{
}

Condition::~Condition()
{
}

void Condition::signal()
{
	Mutex::ScopedLock lock(m_mutex);

	if (!m_waitQueue.empty())
	{
		m_waitQueue.front()->set();
		dequeue();
	}
}

void Condition::broadcast()
{
	Mutex::ScopedLock lock(m_mutex);

	for (WaitQueue::iterator it = m_waitQueue.begin(); it != m_waitQueue.end(); ++it)
	{
		(*it)->set();
	}
	m_waitQueue.clear();
}

void Condition::enqueue(Event& event)
{
	m_waitQueue.push_back(&event);
}

void Condition::dequeue()
{
	m_waitQueue.pop_front();
}

void Condition::dequeue(Event& event)
{
	for (WaitQueue::iterator it = m_waitQueue.begin(); it != m_waitQueue.end(); ++it)
	{
		if (*it == &event)
		{
			m_waitQueue.erase(it);
			break;
		}
	}
}

CUTE_NS_END