// module Thread
#include "Cute/Mutex.h"
#include "Cute/Timestamp.h"

CUTE_NS_BEGIN

#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS - 200112L) >= 0L
#if defined(_POSIX_THREADS) && (_POSIX_THREADS - 200112L) >= 0L
#define CUTE_HAVE_MUTEX_TIMEOUT
#endif
#endif

Mutex::Mutex(MutexType type /* = MUTEX_RECURSIVE */)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
	// the return only checks if the input address of &_cs was valid, so it is safe to omit it
	InitializeCriticalSectionAndSpinCount(&m_mutex, 4000);
#else

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
	pthread_mutexattr_settype_np(&attr, type == MUTEX_RECURSIVE ? PTHREAD_MUTEX_RECURSIVE_NP : PTHREAD_MUTEX_NORMAL_NP);
#elif !defined(CUTE_VXWORKS)
	pthread_mutexattr_settype(&attr, type == MUTEX_RECURSIVE ? PTHREAD_MUTEX_RECURSIVE : PTHREAD_MUTEX_NORMAL);
#endif
	if (pthread_mutex_init(&_mutex, &attr))
	{
		pthread_mutexattr_destroy(&attr);
		throw SystemException("cannot create mutex");
	}
	pthread_mutexattr_destroy(&attr);

#endif
}

Mutex::~Mutex()
{
	mutex_destroy(&m_mutex);
}

bool Mutex::tryLock(long milliseconds)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	const int sleepMillis = 5;
	Timestamp now;
	int64_t diff((int64_t)milliseconds * 1000);

	do
	{
		try
		{
			if (mutex_trylock(&m_mutex))
				return true;
		}
		catch (...)
		{
			throw SystemException("cannot lock mutex");
		}
		Sleep(sleepMillis);
	} while (!now.isElapsed(diff));
	return false;
#else
	//////////////////////////////////////////////////////////////////////////
	// posix
	//////////////////////////////////////////////////////////////////////////
#if defined(CUTE_HAVE_MUTEX_TIMEOUT)
	struct timespec abstime;

	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += milliseconds / 1000;
	abstime.tv_nsec += (milliseconds % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

//#if defined(_POSIX_TIMERS) && defined(CLOCK_REALTIME)
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
	int rc = pthread_mutex_timedlock(&m_mutex, &abstime);
	if (rc == 0)
		return true;
	else if (rc == ETIMEDOUT)
		return false;
	else
		throw SystemException("cannot lock mutex");
#else
	const int sleepMillis = 5;
	Timestamp now;
	int64_t diff(int64_t(milliseconds) * 1000);
	do
	{
		int rc = pthread_mutex_trylock(&m_mutex);
		if (rc == 0)
			return true;
		else if (rc != EBUSY)
			throw SystemException("cannot lock mutex");

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = sleepMillis * 1000;
		select(0, NULL, NULL, NULL, &tv);
#endif
	} while (!now.isElapsed(diff));
	return false;
#endif
}

CUTE_NS_END