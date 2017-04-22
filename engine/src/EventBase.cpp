//! Server
#include "Cute/EventBase.h"
#include "Cute/Clock.h"

CUTE_NS_BEGIN

int64_t EventBase::now()
{
	return Clock::accuracy();
}

EventBase::EventBase()
	: m_prio(PRIO_NORMAL)
	, m_time(0)
{
}

EventBase::~EventBase()
{
}

void EventBase::destroy()
{
	delete this;
}

int64_t EventBase::getTime() const
{
	return m_time;
}

int EventBase::getPriority() const
{
	return m_prio;
}

void EventBase::setDelay(uint32_t delay)
{
	if (delay = 0)
		m_time = 0;
	else
		m_time = now() + delay;
}

void EventBase::setPriority(int prio)
{
	m_prio = prio;
}

bool EventBase::isValid() const
{
	return true;
}

CUTE_NS_END
