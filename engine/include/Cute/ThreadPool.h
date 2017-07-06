#pragma once
#include "Cute/Foundation.h"
#include "Cute/Thread.h"
#include "Cute/Mutex.h"
#include "Cute/Atomic.h"

CUTE_NS_BEGIN

class Runnable;
class PooledThread;

/// A thread pool always keeps a number of threads running, ready
/// to accept work.
/// Creating and starting a threads can impose a significant runtime
/// overhead to an application. A thread pool helps to improve
/// the performance of an application by reducing the number
/// of threads that have to be created (and destroyed again).
/// Threads in a thread pool are re-used once they become
/// available again.
/// The thread pool always keeps a minimum number of threads
/// running. If the demand for threads increases, additional
/// threads are created. Once the demand for threads sinks
/// again, no-longer used threads are stopped and removed
/// from the pool.
class CUTE_CORE_API ThreadPool
{
public:
	enum AffinityPolicy
	{
		TAP_DEFAULT = 0,
		TAP_UNIFORM_DISTRIBUTION,
		TAP_CUSTOM
	};

	ThreadPool(int minCapacity = 2,
		int maxCapacity = 16,
		int idleTime = 60,
		int stackSize = CUTE_THREAD_STACK_SIZE,
		AffinityPolicy affinityPolicy = TAP_DEFAULT);

	ThreadPool(const String& poolName,
		int minCapacity = 2,
		int maxCapacity = 16,
		int idleTime = 60,
		int stackSize = CUTE_THREAD_STACK_SIZE,
		AffinityPolicy affinityPolicy = TAP_DEFAULT);

	~ThreadPool();

	void start(Runnable& target, int cpu = -1);
	void start(Runnable& target, const String& threadName, int cpu = -1);
	void startWithPriority(Thread::Priority priority, Runnable& target, int cpu = -1);
	void startWithPriority(Thread::Priority priority, Runnable& target, const String& threadName, int cpu = -1);

	void stopAll();
	void joinAll();
	void collect();

	const String& name() const;
	int used() const;
	int allocated() const;
	int available() const;

	void addCapacity(int n);
	int capacity() const;

	void setStackSize(int stackSize);
	int getStackSize() const;

	void setAffinityPolicy(AffinityPolicy affinityPolicy);
	AffinityPolicy getAffinityPolicy();

	static ThreadPool& defaultPool(AffinityPolicy affinityPolicy = TAP_DEFAULT);

protected:
	PooledThread* getThread();
	PooledThread* createThread();

	void housekeep();
	int affinity(int cpu);

private:
	ThreadPool(const ThreadPool& pool);
	ThreadPool& operator = (const ThreadPool& pool);

	typedef std::vector<PooledThread*> ThreadVec;

	String			m_name;
	int				m_minCapacity;
	int				m_maxCapacity;
	int				m_idleTime;
	int				m_serial;
	int				m_age;
	int				m_stackSize;
	Atomic32		m_lastCpu;
	ThreadVec		m_threads;
	mutable Mutex	m_mutex;
	AffinityPolicy	m_affinityPolicy;
};

//
// inlines
//
inline void ThreadPool::setStackSize(int stackSize)
{
	m_stackSize = stackSize;
}

inline int ThreadPool::getStackSize() const
{
	return m_stackSize;
}

inline void ThreadPool::setAffinityPolicy(ThreadPool::AffinityPolicy affinityPolicy)
{
	m_affinityPolicy = affinityPolicy;
}

inline ThreadPool::AffinityPolicy ThreadPool::getAffinityPolicy()
{
	return m_affinityPolicy;
}

inline const String& ThreadPool::name() const
{
	return m_name;
}

CUTE_NS_END
