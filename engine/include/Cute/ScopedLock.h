// module Thread
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A class that simplifies thread synchronization
/// with a mutex.
/// The constructor accepts a Mutex (and optionally
/// a timeout value in milliseconds) and locks it.
/// The destructor unlocks the mutex.
template <class M>
class ScopedLock
{
public:
	explicit ScopedLock(M& mutex) 
		: m_mutex(mutex)
	{
		m_mutex.lock();
	}

	ScopedLock(M& mutex, long milliseconds)
		: m_mutex(mutex)
	{
		m_mutex.lock(milliseconds);
	}

	~ScopedLock()
	{
		try
		{
			m_mutex.unlock();
		}
		catch (...)
		{
			cute_unexpected();
		}
	}

private:
	ScopedLock();
	ScopedLock(const ScopedLock&);
	ScopedLock& operator = (const ScopedLock&);

private:
	M& m_mutex;
};

CUTE_NS_END