#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A Semaphore is a synchronization object with the following 
/// characteristics:
/// A semaphore has a value that is constrained to be a non-negative
/// integer and two atomic operations. The allowable operations are V 
/// (here called set()) and P (here called wait()). A V (set()) operation 
/// increases the value of the semaphore by one. 
/// A P (wait()) operation decreases the value of the semaphore by one, 
/// provided that can be done without violating the constraint that the 
/// value be non-negative. A P (wait()) operation that is initiated when 
/// the value of the semaphore is 0 suspends the calling thread. 
/// The calling thread may continue when the value becomes positive again.
class CUTE_CORE_API Semaphore
{
public:
	Semaphore(int n);
	Semaphore(int n, int max);
	~Semaphore();

	/// Increments the semaphore's value by one and
	/// thus signals the semaphore. Another thread
	/// waiting for the semaphore will be able
	/// to continue.
	void set();

	/// Waits for the semaphore to become signalled.
	/// To become signalled, a semaphore's value must
	/// be greater than zero. 
	/// Decrements the semaphore's value by one.
	void wait();

	/// Waits for the semaphore to become signalled.
	/// To become signalled, a semaphore's value must
	/// be greater than zero.
	/// Throws a TimeoutException if the semaphore
	/// does not become signalled within the specified
	/// time interval.
	/// Decrements the semaphore's value by one
	/// if successful.
	void wait(long milliseconds);

	/// Waits for the semaphore to become signalled.
	/// To become signalled, a semaphore's value must
	/// be greater than zero.
	/// Returns true if the semaphore
	/// became signalled within the specified
	/// time interval, false otherwise.
	/// Decrements the semaphore's value by one
	/// if successful.
	bool tryWait(long milliseconds);

private:
	void create(int n, int max);

	Semaphore();
	Semaphore(const Semaphore&);
	Semaphore& operator = (const Semaphore&);

private:
#ifdef CUTE_OS_FAMILY_WINDOWS
	HANDLE			m_sema;
#else
	volatile int    m_n;
	int             m_max;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
#endif
};

CUTE_NS_END
