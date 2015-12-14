#pragma once
#include "API.h"

#ifdef CU_OS_WIN
#	define mutex_t				CRITICAL_SECTION
#	define mutex_init(m)		InitializeCriticalSection(m)
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

CU_NS_BEGIN

// 自旋锁,todo：不依赖atomic实现？
class CU_API SpinLock
{
	std::atomic_flag m_flag;
public:
	SpinLock(){}
	void lock(){
		while (m_flag.test_and_set(std::memory_order_acquire));
	}
	void unlock(){
		m_flag.clear(std::memory_order_release);
	}
};

class CU_API Mutex
{
	CU_NONCOPYABLE(Mutex);

	mutex_t m_handle;
public:
	Mutex(bool recursive = false);
	~Mutex(){ mutex_destroy(&m_handle); }
	inline void lock()  { mutex_lock(&m_handle); }
	inline void unlock(){ mutex_unlock(&m_handle); }
	inline bool try_lock(){ return mutex_trylock(&m_handle); }	// 执行成功时需要unlock

	inline mutex_t* handle() { return &m_handle; }
	inline operator mutex_t()const { return m_handle; }
};

class CU_API NullMutex
{
public:
	NullMutex(){}
	~NullMutex(){}
	inline void lock(){}
	inline void unlock(){}
	inline bool try_lock(){ return true; }
	inline mutex_t* handle() { return NULL; }
};

template<typename T>
class LockGuard
{
	T* m_mutex;
public:
	LockGuard(T& m)
	{
		m_mutex = &m;
		m_mutex->lock();
	}
	LockGuard(T& m, bool need_lock)
	{
		m_mutex = &m;
		if (need_lock)
			m_mutex->lock();
	}

	~LockGuard()
	{
		m_mutex->unlock();
	}
};

typedef LockGuard<Mutex> MutexGuard;

CU_NS_END