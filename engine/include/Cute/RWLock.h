#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class ScopedRWLock;
class ScopedReadRWLock;
class ScopedWriteRWLock;

class CUTE_CORE_API RWLock
{
public:
	typedef ScopedRWLock		ScopedLock;
	typedef ScopedReadRWLock	ScopedReadLock;
	typedef ScopedWriteRWLock	ScopedWriteLock;

	RWLock();
	~RWLock();

	void readLock();
	bool tryReadLock();
	void writeLock();
	bool tryWriteLock();
	void unlock();

#ifdef CUTE_OS_FAMILY_WINDOWS
private:
	void addWriter();
	void removeWriter();
	DWORD tryReadLockOnce();
#endif

private:
	RWLock(const RWLock&);
	RWLock& operator = (const RWLock&);

private:
#ifdef CUTE_OS_FAMILY_WINDOWS
	HANDLE   m_mutex;
	HANDLE   m_readEvent;
	HANDLE   m_writeEvent;
	unsigned m_readers;
	unsigned m_writersWaiting;
	unsigned m_writers;
#else
	pthread_rwlock_t m_rwl;
#endif
};

//////////////////////////////////////////////////////////////////////////
// lock
//////////////////////////////////////////////////////////////////////////
/// A variant of ScopedLock for reader/writer locks.
class CUTE_CORE_API ScopedRWLock
{
public:
	ScopedRWLock(RWLock& rwl, bool write = false)
		: m_rwl(rwl)
	{
		if (write)
			m_rwl.writeLock();
		else
			m_rwl.readLock();
	}

	~ScopedRWLock()
	{
		try
		{
			m_rwl.unlock();
		}
		catch (...)
		{
			cute_unexpected();
		}
	}

private:
	RWLock& m_rwl;

	ScopedRWLock();
	ScopedRWLock(const ScopedRWLock&);
	ScopedRWLock& operator = (const ScopedRWLock&);
};

/// A variant of ScopedLock for reader locks.
class CUTE_CORE_API ScopedReadRWLock : public ScopedRWLock
{
public:
	ScopedReadRWLock(RWLock& rwl):ScopedRWLock(rwl, false){}
	~ScopedReadRWLock(){}
};

/// A variant of ScopedLock for writer locks.
class CUTE_CORE_API ScopedWriteRWLock : public ScopedRWLock
{
public:
	ScopedWriteRWLock(RWLock& rwl) : ScopedRWLock(rwl, true){}
	~ScopedWriteRWLock(){}
};

CUTE_NS_END