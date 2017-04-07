#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

/// An Event is a synchronization object that
/// allows one thread to signal one or more
/// other threads that a certain event
/// has happened.
/// Usually, one thread signals an event,
/// while one or more other threads wait
/// for an event to become signalled.
class CUTE_CORE_API Event
{
public:
	enum EventType
	{
		EVENT_MANUALRESET	= 0,
		EVENT_AUTORESET		= 1
	};

	Event(EventType type = EVENT_AUTORESET);
	~Event();

	/// Signals the event. If autoReset is true,
	/// only one thread waiting for the event 
	/// can resume execution.
	/// If autoReset is false, all waiting threads
	/// can resume execution.
	void set();

	/// Resets the event to unsignalled state.
	void reset();

	/// Waits for the event to become signalled.
	void wait();

	/// Waits for the event to become signalled.
	/// Throws a TimeoutException if the event
	/// does not become signalled within the specified
	/// time interval.
	void wait(long milliseconds);

	/// Waits for the event to become signalled.
	/// Returns true if the event
	/// became signalled within the specified
	/// time interval, false otherwise.
	bool tryWait(long milliseconds);

	void notify()
	{
		set();
	}

private:
	Event(const Event&);
	Event& operator = (const Event&);

#ifdef CUTE_OS_FAMILY_WINDOWS
	HANDLE			m_event;
#else
	bool			m_auto;
	volatile bool	m_state;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
#endif
};

inline void Event::wait(long milliseconds)
{
	if (!tryWait(milliseconds))
		throw TimeoutException();
}

CUTE_NS_END