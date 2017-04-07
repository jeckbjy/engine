// module Thread
#include "Cute/Event.h"

CUTE_NS_BEGIN

Event::Event(EventType type /* = EVENT_AUTORESET */)
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	m_event = CreateEventW(NULL, type == EVENT_AUTORESET ? FALSE : TRUE, FALSE, NULL);
	if (!m_event)
		throw std::exception("cannot create event");
#else
	m_auto = (type == EVENT_AUTORESET);
	m_state = false;

#if defined(CUTE_VXWORKS)
	// This workaround is for VxWorks 5.x where
	// pthread_mutex_init() won't properly initialize the mutex
	// resulting in a subsequent freeze in pthread_mutex_destroy()
	// if the mutex has never been used.
	std::memset(&m_mutex, 0, sizeof(m_mutex));
#endif

	if (pthread_mutex_init(&m_mutex, NULL))
		throw SystemException("cannot create event (mutex)");

#if defined(CUTE_HAVE_MONOTONIC_PTHREAD_COND_TIMEDWAIT)
	pthread_condattr_t attr;
	if (pthread_condattr_init(&attr))
	{
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create event (condition attribute)");
	}
	if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
	{
		pthread_condattr_destroy(&attr);
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create event (condition attribute clock)");
	}
	if (pthread_cond_init(&m_cond, &attr))
	{
		pthread_condattr_destroy(&attr);
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create event (condition)");
	}
	pthread_condattr_destroy(&attr);
#else
	if (pthread_cond_init(&m_cond, NULL))
	{
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create event (condition)");
	}
#endif

#endif
}

Event::~Event()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	CloseHandle(m_event);
#else
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
#endif
}

void Event::set()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	if (!SetEvent(m_event))
		throw SystemException("cannot signal event");
#else
	if (pthread_mutex_lock(&m_mutex))
		throw SystemException("cannot signal event (lock)");
	m_state = true;
	if (pthread_cond_broadcast(&m_cond))
	{
		pthread_mutex_unlock(&m_mutex);
		throw SystemException("cannot signal event");
	}
	pthread_mutex_unlock(&m_mutex);
#endif
}

void Event::reset()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	if (!ResetEvent(m_event))
		throw SystemException("cannot reset event");
#else
	if (pthread_mutex_lock(&m_mutex))
		throw SystemException("cannot reset event");
	m_state = false;
	pthread_mutex_unlock(&m_mutex);
#endif
}

void Event::wait()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	switch (WaitForSingleObject(m_event, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		throw SystemException("wait for event failed");
	}
#else
	if (pthread_mutex_lock(&m_mutex))
		throw SystemException("wait for event failed (lock)");
	while (!m_state)
	{
		if (pthread_cond_wait(&m_cond, &m_mutex))
		{
			pthread_mutex_unlock(&m_mutex);
			throw SystemException("wait for event failed");
		}
	}
	if (m_auto)
		m_state = false;
	pthread_mutex_unlock(&m_mutex);
#endif
}

bool Event::tryWait(long milliseconds)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	switch (WaitForSingleObject(m_event, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw SystemException("wait for event failed");
	}
#else
	//////////////////////////////////////////////////////////////////////////
	// posix
	//////////////////////////////////////////////////////////////////////////
	int rc = 0;
	struct timespec abstime;
	clock_gettime(CLOCK_MONOTONIC, &abstime);

	abstime.tv_sec += milliseconds / 1000;
	abstime.tv_nsec += (milliseconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

//#if defined(__VMS)
//	struct timespec delta;
//	delta.tv_sec = milliseconds / 1000;
//	delta.tv_nsec = (milliseconds % 1000) * 1000000;
//	pthread_get_expiration_np(&delta, &abstime);
//#elif defined(CUTE_HAVE_MONOTONIC_PTHREAD_COND_TIMEDWAIT)
//	clock_gettime(CLOCK_MONOTONIC, &abstime);
//	abstime.tv_sec += milliseconds / 1000;
//	abstime.tv_nsec += (milliseconds % 1000) * 1000000;
//	if (abstime.tv_nsec >= 1000000000)
//	{
//		abstime.tv_nsec -= 1000000000;
//		abstime.tv_sec++;
//	}
//#elif (defined(_POSIX_TIMERS) && defined(CLOCK_REALTIME)) || defined(CUTE_VXWORKS)
//	clock_gettime(CLOCK_REALTIME, &abstime);
//	abstime.tv_sec += milliseconds / 1000;
//	abstime.tv_nsec += (milliseconds % 1000) * 1000000;
//	if (abstime.tv_nsec >= 1000000000)
//	{
//		abstime.tv_nsec -= 1000000000;
//		abstime.tv_sec++;
//	}
//#else
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	abstime.tv_sec = tv.tv_sec + milliseconds / 1000;
//	abstime.tv_nsec = tv.tv_usec * 1000 + (milliseconds % 1000) * 1000000;
//	if (abstime.tv_nsec >= 1000000000)
//	{
//		abstime.tv_nsec -= 1000000000;
//		abstime.tv_sec++;
//	}
//#endif

	if (pthread_mutex_lock(&m_mutex) != 0)
		throw SystemException("wait for event failed (lock)");
	while (!m_state)
	{
		if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			if (rc == ETIMEDOUT)
				break;
			pthread_mutex_unlock(&m_mutex);
			throw SystemException("cannot wait for event");
		}
	}
	if (rc == 0 && m_auto) 
		m_state = false;
	pthread_mutex_unlock(&m_mutex);
	return rc == 0;
#endif
}

CUTE_NS_END