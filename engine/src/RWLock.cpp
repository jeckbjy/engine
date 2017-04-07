// module Thread
#include "Cute/RWLock.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

RWLock::RWLock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	m_mutex = CreateMutexW(NULL, FALSE, NULL);
	if (m_mutex == NULL)
		throw SystemException("cannot create reader/writer lock");

	m_readEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (m_readEvent == NULL)
		throw SystemException("cannot create reader/writer lock");

	m_writeEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
	if (m_writeEvent == NULL)
		throw SystemException("cannot create reader/writer lock");
#else
	if (pthread_rwlock_init(&m_rwl, NULL))
		throw SystemException("cannot create reader/writer lock");
#endif
}

RWLock::~RWLock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	CloseHandle(m_mutex);
	CloseHandle(m_readEvent);
	CloseHandle(m_writeEvent);
#else
	pthread_rwlock_destroy(&m_rwl);
#endif
}

void RWLock::readLock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_readEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++m_readers;
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		cute_assert_dbg(m_writers == 0);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
#else
	if (pthread_rwlock_rdlock(&m_rwl))
		throw SystemException("cannot lock reader/writer lock");
#endif
}

bool RWLock::tryReadLock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	for (;;)
	{
		if (m_writers != 0 || m_writersWaiting != 0)
			return false;

		DWORD result = tryReadLockOnce();
		switch (result)
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			return true;
		case WAIT_TIMEOUT:
			continue; // try again
		default:
			throw SystemException("cannot lock reader/writer lock");
		}
	}
#else
	int rc = pthread_rwlock_tryrdlock(&m_rwl);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
		throw SystemException("cannot lock reader/writer lock");
#endif
}

void RWLock::writeLock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	addWriter();
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--m_writersWaiting;
		++m_readers;
		++m_writers;
		ResetEvent(m_readEvent);
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		cute_assert_dbg(m_writers == 1);
		break;
	default:
		removeWriter();
		throw SystemException("cannot lock reader/writer lock");
	}
#else
	if (pthread_rwlock_wrlock(&m_rwl))
		throw SystemException("cannot lock reader/writer lock");
#endif
}

bool RWLock::tryWriteLock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	addWriter();
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--m_writersWaiting;
		++m_readers;
		++m_writers;
		ResetEvent(m_readEvent);
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		cute_assert_dbg(m_writers == 1);
		return true;
	case WAIT_TIMEOUT:
		removeWriter();
		return false;
	default:
		removeWriter();
		throw SystemException("cannot lock reader/writer lock");
	}
#else
	int rc = pthread_rwlock_trywrlock(&m_rwl);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
		throw SystemException("cannot lock reader/writer lock");
#endif
}

void RWLock::unlock()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	switch (WaitForSingleObject(m_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		m_writers = 0;
		if (m_writersWaiting == 0) SetEvent(m_readEvent);
		if (--m_readers == 0) SetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		break;
	default:
		throw SystemException("cannot unlock reader/writer lock");
	}
#else
	if (pthread_rwlock_unlock(&m_rwl))
		throw SystemException("cannot unlock mutex");
#endif
}

#ifdef CUTE_OS_FAMILY_WINDOWS
inline void RWLock::addWriter()
{
	switch (WaitForSingleObject(m_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (++m_writersWaiting == 1) ResetEvent(m_readEvent);
		ReleaseMutex(m_mutex);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}


inline void RWLock::removeWriter()
{
	switch (WaitForSingleObject(m_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (--m_writersWaiting == 0 && m_writers == 0) SetEvent(m_readEvent);
		ReleaseMutex(m_mutex);
		break;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}

DWORD RWLock::tryReadLockOnce()
{
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_readEvent;
	DWORD result = WaitForMultipleObjects(2, h, TRUE, 1);
	switch (result)
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++m_readers;
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		cute_assert_dbg(m_writers == 0);
		return result;
	case WAIT_TIMEOUT:
		return result;
	default:
		throw SystemException("cannot lock reader/writer lock");
	}
}

#endif

CUTE_NS_END