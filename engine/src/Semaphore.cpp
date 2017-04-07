// module Thread
#include "Cute/Semaphore.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

/// Creates the semaphore. The current value
/// of the semaphore is given in n. The
/// maximum value of the semaphore is given
/// in max.
/// If only n is given, it must be greater than
/// zero.
/// If both n and max are given, max must be
/// greater than zero, n must be greater than
/// or equal to zero and less than or equal
/// to max.
Semaphore::Semaphore(int n)
{
	create(n, n);
}

Semaphore::Semaphore(int n, int max)
{
	create(n, max);
}

Semaphore::~Semaphore()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	CloseHandle(m_sema);
#else
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
#endif
}

void Semaphore::create(int n, int max)
{
	cute_assert(n >= 0 && max > 0 && n <= max);

#ifdef CUTE_OS_FAMILY_WINDOWS
	m_sema = CreateSemaphoreW(NULL, n, max, NULL);
	if (!m_sema)
	{
		throw SystemException("cannot create semaphore");
	}
#else
#if defined(POCO_VXWORKS)
	// This workaround is for VxWorks 5.x where
	// pthread_mutex_init() won't properly initialize the mutex
	// resulting in a subsequent freeze in pthread_mutex_destroy()
	// if the mutex has never been used.
	std::memset(&m_mutex, 0, sizeof(m_mutex));
#endif
	if (pthread_mutex_init(&m_mutex, NULL))
		throw SystemException("cannot create semaphore (mutex)");

#if defined(CUTE_HAVE_MONOTONIC_PTHREAD_COND_TIMEDWAIT)
	pthread_condattr_t attr;
	if (pthread_condattr_init(&attr))
	{
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create semaphore (condition attribute)");
	}
	if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
	{
		pthread_condattr_destroy(&attr);
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create semaphore (condition attribute clock)");
	}
	if (pthread_cond_init(&m_cond, &attr))
	{
		pthread_condattr_destroy(&attr);
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create semaphore (condition)");
	}
	pthread_condattr_destroy(&attr);
#else
	if (pthread_cond_init(&m_cond, NULL))
	{
		pthread_mutex_destroy(&m_mutex);
		throw SystemException("cannot create semaphore (condition)");
	}
#endif
#endif
}

void Semaphore::set()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	if (!ReleaseSemaphore(m_sema, 1, NULL))
	{
		throw SystemException("cannot signal semaphore");
	}
#else
	if (pthread_mutex_lock(&m_mutex))
		throw SystemException("cannot signal semaphore (lock)");
	if (m_n < m_max)
	{
		++m_n;
	}
	else
	{
		pthread_mutex_unlock(&m_mutex);
		throw SystemException("cannot signal semaphore: count would exceed maximum");
	}
	if (pthread_cond_signal(&m_cond))
	{
		pthread_mutex_unlock(&m_mutex);
		throw SystemException("cannot signal semaphore");
	}
	pthread_mutex_unlock(&m_mutex);
#endif
}

void Semaphore::wait()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	switch (WaitForSingleObject(m_sema, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		throw SystemException("wait for semaphore failed");
	}
#else
	if (pthread_mutex_lock(&m_mutex))
		throw SystemException("wait for semaphore failed (lock)");
	while (m_n < 1)
	{
		if (pthread_cond_wait(&m_cond, &m_mutex))
		{
			pthread_mutex_unlock(&m_mutex);
			throw SystemException("wait for semaphore failed");
		}
	}
	--m_n;
	pthread_mutex_unlock(&m_mutex);
#endif
}

void Semaphore::wait(long milliseconds)
{
	if (!tryWait(milliseconds))
		throw TimeoutException();
}

bool Semaphore::tryWait(long milliseconds)
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	switch (WaitForSingleObject(m_sema, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		throw SystemException("wait for semaphore failed");
	}
#else
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
/*
#if defined(__VMS)
	struct timespec delta;
	delta.tv_sec = milliseconds / 1000;
	delta.tv_nsec = (milliseconds % 1000) * 1000000;
	pthread_get_expiration_np(&delta, &abstime);
#elif defined(CUTE_HAVE_MONOTONIC_PTHREAD_COND_TIMEDWAIT)
	clock_gettime(CLOCK_MONOTONIC, &abstime);
	abstime.tv_sec += milliseconds / 1000;
	abstime.tv_nsec += (milliseconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}
#elif (defined(_POSIX_TIMERS) && defined(CLOCK_REALTIME)) || defined(POCO_VXWORKS)
	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += milliseconds / 1000;
	abstime.tv_nsec += (milliseconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec = tv.tv_sec + milliseconds / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (milliseconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}
#endif*/

	if (pthread_mutex_lock(&m_mutex) != 0)
		throw SystemException("wait for semaphore failed (lock)");
	while (m_n < 1)
	{
		if ((rc = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime)))
		{
			if (rc == ETIMEDOUT) break;
			pthread_mutex_unlock(&m_mutex);
			throw SystemException("cannot wait for semaphore");
		}
	}
	if (rc == 0) --m_n;
	pthread_mutex_unlock(&m_mutex);
	return rc == 0;
#endif
}

CUTE_NS_END