// module Thread
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A class that simplifies thread synchronization
/// with a mutex.
/// The constructor accepts a Mutex and unlocks it.
/// The destructor locks the mutex.
template <class M>
class ScopedUnlock
{
public:
	inline ScopedUnlock(M& mutex, bool unlockNow = true) : m_mutex(mutex)
	{
		if (unlockNow)
			m_mutex.unlock();
	}
	inline ~ScopedUnlock()
	{
		try
		{
			m_mutex.lock();
		}
		catch (...)
		{
			cute_unexpected();
		}
	}

private:
	ScopedUnlock();
	ScopedUnlock(const ScopedUnlock&);
	ScopedUnlock& operator = (const ScopedUnlock&);

private:
	M& m_mutex;
};

CUTE_NS_END
