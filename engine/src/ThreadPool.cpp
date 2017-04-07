// module Thread
#include "Cute/ThreadPool.h"
#include "Cute/Runnable.h"
#include "Cute/Thread.h"
#include "Cute/Event.h"
#include "Cute/ThreadLocal.h"
#include "Cute/ErrorHandler.h"
#include "Cute/Environment.h"

#include <sstream>
#include <ctime>

CUTE_NS_BEGIN

class PooledThread : public Runnable
{
public:
	PooledThread(const std::string& name, int stackSize = 0);
	~PooledThread();

	void start(int cpu = -1);
	void start(Thread::Priority priority, Runnable& target, int cpu = -1);
	void start(Thread::Priority priority, Runnable& target, const std::string& name, int cpu = -1);
	bool idle();
	int idleTime();
	void join();
	void activate();
	void release();
	void run();

private:
	volatile bool   m_idle;
	volatile time_t	m_idleTime;
	Runnable*		m_target;
	String			m_name;
	Thread          m_thread;
	Event           m_targetReady;
	Event           m_targetCompleted;
	Event           m_started;
	Mutex	        m_mutex;
};

PooledThread::PooledThread(const std::string& name, int stackSize) 
	: m_idle(true)
	, m_idleTime(0)
	, m_target(0)
	, m_name(name)
	, m_thread(name)
	, m_targetReady()
	, m_targetCompleted(Event::EVENT_MANUALRESET)
	, m_started()
	, m_mutex()
{
	cute_assert_dbg(stackSize >= 0);
	m_thread.setStackSize(stackSize);
	m_idleTime = time(NULL);
}

PooledThread::~PooledThread()
{
}

void PooledThread::start(int cpu)
{
	m_thread.start(*this);
	m_started.wait();
	if (cpu >= 0)
	{
		m_thread.setAffinity(static_cast<unsigned>(cpu));
	}
}

void PooledThread::start(Thread::Priority priority, Runnable& target, int cpu)
{
	Mutex::ScopedLock lock(m_mutex);

	cute_assert(m_target == 0);

	m_target = &target;
	m_thread.setPriority(priority);
	m_targetReady.set();
	if (cpu >= 0)
	{
		m_thread.setAffinity(static_cast<unsigned>(cpu));
	}
}

void PooledThread::start(Thread::Priority priority, Runnable& target, const std::string& name, int cpu)
{
	Mutex::ScopedLock lock(m_mutex);

	std::string fullName(name);
	if (name.empty())
	{
		fullName = m_name;
	}
	else
	{
		fullName.append(" (");
		fullName.append(m_name);
		fullName.append(")");
	}
	m_thread.setName(fullName);
	m_thread.setPriority(priority);

	cute_assert(m_target == 0);

	m_target = &target;
	m_targetReady.set();
	if (cpu >= 0)
	{
		m_thread.setAffinity(static_cast<unsigned>(cpu));
	}
}

inline bool PooledThread::idle()
{
	Mutex::ScopedLock lock(m_mutex);
	return m_idle;
}

int PooledThread::idleTime()
{
	Mutex::ScopedLock lock(m_mutex);
	return (int)(time(NULL) - m_idleTime);	
}

void PooledThread::join()
{
	m_mutex.lock();
	Runnable* pTarget = m_target;
	m_mutex.unlock();
	if (pTarget)
		m_targetCompleted.wait();
}

void PooledThread::activate()
{
	Mutex::ScopedLock lock(m_mutex);

	cute_assert(m_idle);
	m_idle = false;
	m_targetCompleted.reset();
}

void PooledThread::release()
{
	const long JOIN_TIMEOUT = 10000;

	m_mutex.lock();
	m_target = 0;
	m_mutex.unlock();
	// In case of a statically allocated thread pool (such
	// as the default thread pool), Windows may have already
	// terminated the thread before we got here.
	if (m_thread.isRunning())
		m_targetReady.set();

	if (m_thread.join(JOIN_TIMEOUT))
	{
		delete this;
	}
}

void PooledThread::run()
{
	m_started.set();
	for (;;)
	{
		m_targetReady.wait();
		m_mutex.lock();
		if (m_target) // a NULL target means kill yourself
		{
			Runnable* pTarget = m_target;
			m_mutex.unlock();
			try
			{
				pTarget->run();
			}
			catch (Exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (std::exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (...)
			{
				ErrorHandler::handle();
			}
			Mutex::ScopedLock lock(m_mutex);
			m_target = 0;
			m_idleTime = time(NULL);
			m_idle = true;
			m_targetCompleted.set();
			ThreadLocalStorage::clear();
			m_thread.setName(m_name);
			m_thread.setPriority(Thread::PRIO_NORMAL);
		}
		else
		{
			m_mutex.unlock();
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
ThreadPool::ThreadPool(int minCapacity, int maxCapacity, int idleTime, int stackSize, AffinityPolicy affinityPolicy) 
	: m_minCapacity(minCapacity)
	, m_maxCapacity(maxCapacity)
	, m_idleTime(idleTime)
	, m_serial(0)
	, m_age(0)
	, m_stackSize(stackSize)
	, m_lastCpu(0)
	, m_affinityPolicy(affinityPolicy)
{
	cute_assert(minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

	int cpu = -1;
	int cpuCount = Environment::processorCount();

	for (int i = 0; i < m_minCapacity; i++)
	{
		if (m_affinityPolicy == TAP_UNIFORM_DISTRIBUTION)
		{
			cpu = m_lastCpu.value() % cpuCount;
			m_lastCpu++;
		}
		PooledThread* pThread = createThread();
		m_threads.push_back(pThread);
		pThread->start(cpu);
	}
}

ThreadPool::ThreadPool(const String& name, int minCapacity, int maxCapacity, int idleTime, int stackSize, AffinityPolicy affinityPolicy)
	: m_name(name)
	, m_minCapacity(minCapacity)
	, m_maxCapacity(maxCapacity)
	, m_idleTime(idleTime)
	, m_serial(0)
	, m_age(0)
	, m_stackSize(stackSize)
	, m_lastCpu(0)
	, m_affinityPolicy(affinityPolicy)
{
	cute_assert(minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

	int cpu = -1;
	int cpuCount = Environment::processorCount();
	for (int i = 0; i < m_minCapacity; i++)
	{
		if (m_affinityPolicy == TAP_UNIFORM_DISTRIBUTION)
		{
			cpu = m_lastCpu.value() % cpuCount;
			m_lastCpu++;
		}
		PooledThread* pThread = createThread();
		m_threads.push_back(pThread);
		pThread->start(cpu);
	}
}

ThreadPool::~ThreadPool()
{
	try
	{
		stopAll();
	}
	catch (...)
	{
		cute_unexpected();
	}
}

void ThreadPool::addCapacity(int n)
{
	Mutex::ScopedLock lock(m_mutex);

	cute_assert(m_maxCapacity + n >= m_minCapacity);
	m_maxCapacity += n;
	housekeep();
}

int ThreadPool::capacity() const
{
	Mutex::ScopedLock lock(m_mutex);
	return m_maxCapacity;
}

int ThreadPool::available() const
{
	Mutex::ScopedLock lock(m_mutex);

	int count = 0;
	for (ThreadVec::const_iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		if ((*it)->idle()) ++count;
	}
	return (int)(count + m_maxCapacity - m_threads.size());
}

int ThreadPool::used() const
{
	Mutex::ScopedLock lock(m_mutex);

	int count = 0;
	for (ThreadVec::const_iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		if (!(*it)->idle()) ++count;
	}
	return count;
}

int ThreadPool::allocated() const
{
	Mutex::ScopedLock lock(m_mutex);

	return int(m_threads.size());
}

int ThreadPool::affinity(int cpu)
{
	switch (static_cast<int>(m_affinityPolicy))
	{
	case TAP_UNIFORM_DISTRIBUTION:
	{
		cpu = m_lastCpu.value() % Environment::processorCount();
		m_lastCpu++;
	}
	break;
	case TAP_DEFAULT:
	{
		cpu = -1;
	}
	break;
	case TAP_CUSTOM:
	{
		if ((cpu < -1) || (cpu >= Environment::processorCount()))
		{
			throw InvalidArgumentException("cpu argument is invalid");
		}
	}
	break;
	}
	return cpu;
}

void ThreadPool::start(Runnable& target, int cpu)
{
	getThread()->start(Thread::PRIO_NORMAL, target, affinity(cpu));
}

void ThreadPool::start(Runnable& target, const std::string& rName, int cpu)
{
	getThread()->start(Thread::PRIO_NORMAL, target, rName, affinity(cpu));
}

void ThreadPool::startWithPriority(Thread::Priority priority, Runnable& target, int cpu)
{
	getThread()->start(priority, target, affinity(cpu));
}

void ThreadPool::startWithPriority(Thread::Priority priority, Runnable& target, const std::string& rName, int cpu)
{
	getThread()->start(priority, target, rName, affinity(cpu));
}

/// Stops all running threads and waits for their completion.
///
/// Will also delete all thread objects.
/// If used, this method should be the last action before
/// the thread pool is deleted.
///
/// Note: If a thread fails to stop within 10 seconds 
/// (due to a programming error, for example), the
/// underlying thread object will not be deleted and
/// this method will return anyway. This allows for a
/// more or less graceful shutdown in case of a misbehaving
/// thread.
void ThreadPool::stopAll()
{
	Mutex::ScopedLock lock(m_mutex);

	for (ThreadVec::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		(*it)->release();
	}
	m_threads.clear();
}

/// Waits for all threads to complete.
///
/// Note that this will not actually join() the underlying
/// thread, but rather wait for the thread's runnables
/// to finish.
void ThreadPool::joinAll()
{
	Mutex::ScopedLock lock(m_mutex);

	for (ThreadVec::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		(*it)->join();
	}
	housekeep();
}

/// Stops and removes no longer used threads from the
/// thread pool. Can be called at various times in an
/// application's life time to help the thread pool
/// manage its threads. Calling this method is optional,
/// as the thread pool is also implicitly managed in
/// calls to start(), addCapacity() and joinAll().
void ThreadPool::collect()
{
	Mutex::ScopedLock lock(m_mutex);
	housekeep();
}

void ThreadPool::housekeep()
{
	m_age = 0;
	if (m_threads.size() <= m_minCapacity)
		return;

	ThreadVec idleThreads;
	ThreadVec expiredThreads;
	ThreadVec activeThreads;
	idleThreads.reserve(m_threads.size());
	activeThreads.reserve(m_threads.size());

	for (ThreadVec::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		if ((*it)->idle())
		{
			if ((*it)->idleTime() < m_idleTime)
				idleThreads.push_back(*it);
			else
				expiredThreads.push_back(*it);
		}
		else activeThreads.push_back(*it);
	}
	int n = (int)activeThreads.size();
	int limit = (int)idleThreads.size() + n;
	if (limit < m_minCapacity) limit = m_minCapacity;
	idleThreads.insert(idleThreads.end(), expiredThreads.begin(), expiredThreads.end());
	m_threads.clear();
	for (ThreadVec::iterator it = idleThreads.begin(); it != idleThreads.end(); ++it)
	{
		if (n < limit)
		{
			m_threads.push_back(*it);
			++n;
		}
		else (*it)->release();
	}
	m_threads.insert(m_threads.end(), activeThreads.begin(), activeThreads.end());
}

PooledThread* ThreadPool::getThread()
{
	Mutex::ScopedLock lock(m_mutex);

	if (++m_age == 32)
		housekeep();

	PooledThread* pThread = 0;
	for (ThreadVec::iterator it = m_threads.begin(); !pThread && it != m_threads.end(); ++it)
	{
		if ((*it)->idle())
			pThread = *it;
	}
	if (!pThread)
	{
		if (m_threads.size() < m_maxCapacity)
		{
			pThread = createThread();
			try
			{
				pThread->start();
				m_threads.push_back(pThread);
			}
			catch (...)
			{
				delete pThread;
				throw;
			}
		}
		else throw NoThreadAvailableException();
	}
	pThread->activate();
	return pThread;
}

PooledThread* ThreadPool::createThread()
{
	std::ostringstream threadName;
	threadName << m_name << "[#" << ++m_serial << "]";
	return new PooledThread(threadName.str(), m_stackSize);
}

class ThreadPoolSingletonHolder
{
public:
	ThreadPoolSingletonHolder()
	{
		m_pool = 0;
	}

	~ThreadPoolSingletonHolder()
	{
		delete m_pool;
	}

	ThreadPool* pool(ThreadPool::AffinityPolicy affinityPolicy = ThreadPool::TAP_DEFAULT)
	{
		Mutex::ScopedLock lock(_mutex);

		if (!m_pool)
		{
			m_pool = new ThreadPool("default");
			m_pool->setAffinityPolicy(affinityPolicy);
			if (CUTE_THREAD_STACK_SIZE > 0)
				m_pool->setStackSize(CUTE_THREAD_STACK_SIZE);
		}
		return m_pool;
	}

private:
	ThreadPool* m_pool;
	Mutex		_mutex;
};

namespace
{
	static ThreadPoolSingletonHolder sh;
}

ThreadPool& ThreadPool::defaultPool(AffinityPolicy affinityPolicy)
{
	return *sh.pool(affinityPolicy);
}

CUTE_NS_END