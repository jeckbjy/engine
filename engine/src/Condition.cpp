#include "Condition.h"

CU_NS_BEGIN

#ifdef CU_OS_WIN

#define COND_EVENT_ONE 0
#define COND_EVENT_ALL 1

void Condition::pthread_cond_init(pthread_cond_t* handle, void*)
{
	handle->waiters = 0;
	handle->events[COND_EVENT_ONE] = CreateEvent(NULL, FALSE, FALSE, NULL);
	handle->events[COND_EVENT_ALL] = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&handle->mutex);
}

void Condition::pthread_cond_destroy(pthread_cond_t* handle)
{
	CloseHandle(handle->events[COND_EVENT_ONE]);
	CloseHandle(handle->events[COND_EVENT_ALL]);
	DeleteCriticalSection(&handle->mutex);
}

void Condition::pthread_cond_signal(pthread_cond_t* handle)
{
	// Are there any waiters?
	EnterCriticalSection(&handle->mutex);
	bool haveWaiters = (handle->waiters > 0);
	LeaveCriticalSection(&handle->mutex);

	// If we have any waiting threads, send them a signal
	if (haveWaiters)
		SetEvent(handle->events[COND_EVENT_ONE]);
}

void Condition::pthread_cond_broadcast(pthread_cond_t* handle)
{
	// Are there any waiters?
	EnterCriticalSection(&handle->mutex);
	bool haveWaiters = (handle->waiters > 0);
	LeaveCriticalSection(&handle->mutex);

	// If we have any waiting threads, send them a signal
	if (haveWaiters)
		SetEvent(handle->events[COND_EVENT_ALL]);
}

void Condition::pthread_cond_wait(pthread_cond_t* handle, CRITICAL_SECTION* mutex)
{
	// Increment number of waiters
	EnterCriticalSection(&handle->mutex);
	++handle->waiters;
	LeaveCriticalSection(&handle->mutex);

	// Release the mutex while waiting for the condition (will decrease the number of waiters when done)...
	LeaveCriticalSection(mutex);

	// Wait for either event to become signaled due to notify_one() or
	// notify_all() being called
	int result = WaitForMultipleObjects(2, handle->events, FALSE, INFINITE);

	// Check if we are the last waiter
	EnterCriticalSection(&handle->mutex);
	--handle->waiters;
	bool lastWaiter = (result == (WAIT_OBJECT_0 + COND_EVENT_ALL)) && (handle->waiters == 0);
	LeaveCriticalSection(&handle->mutex);

	// If we are the last waiter to be notified to stop waiting, reset the event
	if (lastWaiter)
		ResetEvent(handle->events[COND_EVENT_ALL]);
	// lock
	EnterCriticalSection(mutex);
}

#endif

CU_NS_END