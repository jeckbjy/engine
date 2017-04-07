#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"
#include "Cute/ScopedLock.h"

#ifdef CUTE_OS_FAMILY_WINDOWS
#	define mutex_t				CRITICAL_SECTION
#	define mutex_init(m)		InitializeCriticalSectionAndSpinCount(m, 4000)
#	define mutex_destroy(m)		DeleteCriticalSection(m)
#	define mutex_lock(m)		EnterCriticalSection(m)
#	define mutex_unlock(m)		LeaveCriticalSection(m)
#	define mutex_trylock(m)		TryEnterCriticalSection(m) == TRUE
#else
#	define mutex_t				pthread_mutex_t
#	define mutex_init(m)		pthread_mutex_init(m, NULL)
#	define mutex_destroy(m)		pthread_mutex_destroy(m)
#	define mutex_lock(m)		pthread_mutex_lock(m)
#	define mutex_unlock(m)		pthread_mutex_unlock(m)
#	define mutex_trylock(m)		pthread_mutex_trylock(m) == 0
#endif

CUTE_NS_BEGIN

/// A Mutex (mutual exclusion) is a synchronization 
/// mechanism used to control access to a shared resource
/// in a concurrent (multithreaded) scenario.
/// Using the ScopedLock class is the preferred way to automatically
/// lock and unlock a mutex.
/// Mutex is always RECURSIVE in windows!!!
class CUTE_CORE_API Mutex
{
public:
	typedef ScopedLock<Mutex> ScopedLock;

	enum MutexType
	{
		MUTEX_NONRECURSIVE	= 0,
		MUTEX_RECURSIVE		= 1,
	};

	Mutex(MutexType type = MUTEX_RECURSIVE);
	~Mutex();

	inline void lock()
	{
		mutex_lock(&m_mutex);
	}
	inline void lock(long milliseconds)
	{
		if (!tryLock(milliseconds))
			throw TimeoutException();
	}
	inline bool tryLock()
	{
		return mutex_trylock(&m_mutex);
	}
	inline void unlock()
	{
		mutex_unlock(&m_mutex);
	}

	bool tryLock(long milliseconds);

private:
	Mutex(const Mutex&);
	Mutex& operator = (const Mutex&);

private:
	mutex_t m_mutex;
};

class CUTE_CORE_API NullMutex
{
public:
	typedef ScopedLock<NullMutex> ScopedLock;

	NullMutex()
	{
	}

	~NullMutex()
	{
	}

	void lock()
	{
	}

	void lock(long)
	{
	}

	bool tryLock()
	{
		return true;
	}

	bool tryLock(long)
	{
		return true;
	}

	void unlock()
	{
	}
};

CUTE_NS_END