//! Server
#include "Cute/LogicEvent.h"
#include "Cute/Clock.h"

CUTE_NS_BEGIN

int64_t LogicEvent::now()
{
	return Clock::accuracy();
}

LogicEvent::LogicEvent()
	: m_prio(PRIO_NORMAL)
	, m_time(0)
{
}

LogicEvent::~LogicEvent()
{
}

void LogicEvent::destroy()
{
	delete this;
}

int64_t LogicEvent::getTime() const
{
	return m_time;
}

int LogicEvent::getPriority() const
{
	return m_prio;
}

void LogicEvent::setDelay(uint32_t delay)
{
	if (delay = 0)
		m_time = 0;
	else
		m_time = now() + delay;
}

void LogicEvent::setPriority(int prio)
{
	m_prio = prio;
}

bool LogicEvent::isValid() const
{
	return true;
}

CUTE_NS_END
