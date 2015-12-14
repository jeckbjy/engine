#include "SyncEvent.h"

CU_NS_BEGIN

SyncEvent::SyncEvent(bool auto_reset /* = true */)
{
#ifdef CU_OS_WINNT
	m_handle = CreateEventW(NULL, auto_reset ? FALSE : TRUE, FALSE, NULL);
	if (!m_handle)
		throw std::exception("cannot create event");
#else
	m_auto = auto_reset;
	m_state = false;
	if (pthread_mutex_init(&m_mutex, NULL))
		throw std::runtime_error("cannot create event (mutex)");
	if (pthread_cond_init(&m_cond, NULL))
		throw std::runtime_error("cannot create event (condition)");
#endif
}

SyncEvent::~SyncEvent()
{
#ifdef CU_OS_WINNT
	CloseHandle(m_handle);
#else
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
#endif
}

void SyncEvent::set()
{
#ifdef CU_OS_WINNT
	if (!SetEvent(m_handle))
		throw std::exception("cannot signal event");
#else
	if (pthread_mutex_lock(&m_mutex))
		throw std::runtime_error("cannot signal event (lock)");
	m_state = true;
	if (pthread_cond_broadcast(&m_cond))
	{
		pthread_mutex_unlock(&m_mutex);
		throw std::runtime_error("cannot signal event");
	}
	pthread_mutex_unlock(&m_mutex);
#endif
}

void SyncEvent::reset()
{
#ifdef CU_OS_WINNT
	if (!ResetEvent(m_handle))
		throw std::exception("cannot reset event");
#else
	if (pthread_mutex_lock(&m_mutex))
		throw std::runtime_error("cannot reset event");
	m_state = false;
	pthread_mutex_unlock(&m_mutex);
#endif
}

void SyncEvent::wait()
{
#ifdef CU_OS_WINNT
	if (WaitForSingleObject(m_handle, INFINITE) != WAIT_OBJECT_0)
		throw std::exception("wait for event failed");
#else
	if (pthread_mutex_lock(&m_mutex))
		throw std::runtime_error("wait for event failed (lock)");
	while (!m_state)
	{
		if (pthread_cond_wait(&m_cond, &m_mutex))
		{
			pthread_mutex_unlock(&m_mutex);
			throw std::runtime_error("wait for event failed");
		}
	}
	if (m_auto)
		m_state = false;
	pthread_mutex_unlock(&m_mutex);
#endif
}

void SyncEvent::wait(long msec)
{
	if (!try_wait(msec))
		throw std::exception("wait for event timeout");
}

bool SyncEvent::try_wait(long msec)
{
#ifdef CU_OS_WINNT
	switch (WaitForSingleObject(m_handle, msec + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw std::exception("wait for event failed");
	}
#else
	int rc = 0;
	struct timespec abstime;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec = tv.tv_sec + milliseconds / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (milliseconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	if (pthread_mutex_lock(&m_mutex) != 0)
		throw std::runtime_error("wait for event failed (lock)");
	while (!m_state)
	{
		if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			if (rc == ETIMEDOUT) break;
			pthread_mutex_unlock(&m_mutex);
			throw std::runtime_error("cannot wait for event");
		}
	}
	if (rc == 0 && m_auto)
		m_state = false;
	pthread_mutex_unlock(&m_mutex);
	return rc == 0;
#endif
}

CU_NS_END