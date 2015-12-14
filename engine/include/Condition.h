#pragma once
#include "Mutex.h"

CU_NS_BEGIN

/// Condition variable class.
/// This is a signalling object for synchronizing the execution flow for
/// several threads. Example usage:
/// @code
/// // Shared data and associated mutex and condition variable objects
/// int count;
/// mutex m;
/// condition_variable cond;
///
/// // Wait for the counter to reach a certain number
/// void wait_counter(int targetCount)
/// {
///   lock_guard<mutex> guard(m);
///   while(count < targetCount)
///     cond.wait(m);
/// }
///
/// // Increment the counter, and notify waiting threads
/// void increment()
/// {
///   lock_guard<mutex> guard(m);
///   ++ count;
///   cond.notify_all();
/// }
/// @endcode
class CU_API Condition
{
#ifdef CU_OS_WIN
	struct pthread_cond_t
	{
		HANDLE		events[2];
		uint32_t	waiters;
		CRITICAL_SECTION mutex;
	};
	static void pthread_cond_init(pthread_cond_t* handle, void*);
	static void pthread_cond_destroy(pthread_cond_t* handle);
	static void pthread_cond_signal(pthread_cond_t* handle);
	static void pthread_cond_broadcast(pthread_cond_t* handle);
	static void pthread_cond_wait(pthread_cond_t* handle, CRITICAL_SECTION* mutex);
#endif
public:
	Condition()  { pthread_cond_init(&m_handle, NULL); }
	~Condition() { pthread_cond_destroy(&m_handle); }

	inline void notify_one() { pthread_cond_signal(&m_handle); }
	inline void notify_all() { pthread_cond_broadcast(&m_handle); }
	template<class TMutex>
	inline void wait(TMutex& mutex) { pthread_cond_wait(&m_handle, mutex.handle()); }

private:
	pthread_cond_t m_handle;
};

CU_NS_END