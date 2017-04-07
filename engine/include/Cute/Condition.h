#pragma once
#include "Cute/Foundation.h"
#include "Cute/Mutex.h"
#include "Cute/Event.h"

CUTE_NS_BEGIN

/// A Condition is a synchronization object used to block a thread 
/// until a particular condition is met. 
/// A Condition object is always used in conjunction with
/// a Mutex (or FastMutex) object.
///
/// Condition objects are similar to POSIX condition variables, which the
/// difference that Condition is not subject to spurious wakeups.
///
/// Threads waiting on a Condition are resumed in FIFO order.
class CUTE_CORE_API Condition
{
public:
	Condition();
	~Condition();

	/// Unlocks the mutex (which must be locked upon calling
	/// wait()) and waits until the Condition is signalled.
	///
	/// The given mutex will be locked again upon 
	/// leaving the function, even in case of an exception.
	template <class Mtx>
	void wait(Mtx& mutex)
	{
		ScopedUnlock<Mtx> unlock(mutex, false);
		Event event;
		{
			Mutex::ScopedLock lock(m_mutex);
			mutex.unlock();
			enqueue(event);
		}
		event.wait();
	}

	/// Unlocks the mutex (which must be locked upon calling
	/// wait()) and waits for the given time until the Condition is signalled.
	///
	/// The given mutex will be locked again upon successfully leaving the 
	/// function, even in case of an exception.
	///
	/// Throws a TimeoutException if the Condition is not signalled
	/// within the given time interval.
	template <class Mtx>
	void wait(Mtx& mutex, long milliseconds)
	{
		if (!tryWait(mutex, milliseconds))
			throw TimeoutException();
	}

	/// Unlocks the mutex (which must be locked upon calling
	/// tryWait()) and waits for the given time until the Condition is signalled.
	///
	/// The given mutex will be locked again upon leaving the 
	/// function, even in case of an exception.
	///
	/// Returns true if the Condition has been signalled
	/// within the given time interval, otherwise false.
	template <class Mtx>
	bool tryWait(Mtx& mutex, long milliseconds)
	{
		ScopedUnlock<Mtx> unlock(mutex, false);
		Event event;
		{
			Mutex::ScopedLock lock(m_mutex);
			mutex.unlock();
			enqueue(event);
		}
		if (!event.tryWait(milliseconds))
		{
			Mutex::ScopedLock lock(m_mutex);
			dequeue(event);
			return false;
		}
		return true;
	}

	void signal();
	/// Signals the Condition and allows one waiting thread
	/// to continue execution.

	void broadcast();
	/// Signals the Condition and allows all waiting
	/// threads to continue their execution.

protected:
	void enqueue(Event& event);
	void dequeue();
	void dequeue(Event& event);

private:
	Condition(const Condition&);
	Condition& operator = (const Condition&);

	typedef std::deque<Event*> WaitQueue;

	Mutex		m_mutex;
	WaitQueue	m_waitQueue;
};

CUTE_NS_END