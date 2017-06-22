// module Thread
#include "Cute/Thread.h"
#include "Cute/Runnable.h"
#include "Cute/Exception.h"
#include "Cute/ThreadLocal.h"
#include "Cute/ErrorHandler.h"
#include "Cute/Atomic.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// Block SIGPIPE in main thread
//////////////////////////////////////////////////////////////////////////
#if defined(CUTE_OS_FAMILY_POSIX) && !defined(CUTE_VXWORKS)
namespace
{
    class SignalBlocker
    {
    public:
        SignalBlocker()
        {
            sigset_t sset;
            sigemptyset(&sset);
            sigaddset(&sset, SIGPIPE);
            pthread_sigmask(SIG_BLOCK, &sset, 0);
        }
        ~SignalBlocker()
        {
        }
    };
    
    static SignalBlocker signalBlocker;
}
#endif

#if defined(CUTE_POSIX_DEBUGGER_THREAD_NAMES)
namespace {
    void setThreadName(pthread_t thread, const String& threadName)
    {
#if (CUTE_OS == POCO_OS_MAC_OS_X)
        pthread_setname_np(threadName.c_str()); // __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_2)
#else
        if (pthread_setname_np(thread, threadName.c_str()) == ERANGE && threadName.size() > 15)
        {
            std::string truncName(threadName, 0, 7);
            truncName.append("~");
            truncName.append(threadName, threadName.size() - 7, 7);
            pthread_setname_np(thread, truncName.c_str());
        }
#endif
    }
}

#endif // CUTE_POSIX_DEBUGGER_THREAD_NAMES

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#	define __key_t				DWORD
#	define __key_create(key)	(key = TlsAlloc()) == TLS_OUT_OF_INDEXES
#	define __key_free(key)		TlsFree(key)
#	define __key_get(key)		TlsGetValue(key)
#	define __key_set(k,v)		TlsSetValue(k,v)
#else
#	define __key_t				pthread_key_t
#	define __key_create(key)	pthread_key_create(&key, NULL)
#	define __key_free(key)		pthread_key_delete(key)
#	define __key_get(key)		pthread_getspecific(key)
#	define __key_set(k,v)		pthread_setspecific(k,v)
#endif

__RET __API RunnableEntry(void* args)
{
#if defined(CUTE_OS_FAMILY_POSIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

	Thread* pThread = (Thread*)(args);

#if defined(CUTE_POSIX_DEBUGGER_THREAD_NAMES)
    setThreadName(pThread->m_thread, pThread->getName());
#endif
#if CUTE_OS == CUTE_OS_LINUX
	pThread->m_tid = (thread_id)syscall(SYS_gettid);
#endif
	try
	{
		pThread->run();
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
    
#if defined(CUTE_OS_FAMILY_POSIX)
    pThread->m_done.set();
#endif

	return 0;
}

class ThreadHolder
{
	__key_t _key;
public:
	ThreadHolder()
	{
		if (__key_create(_key))
			throw SystemException("cannot allocate thread context key");
	}

	~ThreadHolder()
	{
		__key_free(_key); 
	}

	Thread* get() const
	{
		return reinterpret_cast<Thread*>(__key_get(_key)); 
	}

	void set(Thread* data)
	{
		__key_set(_key, data); 
	}
};

namespace{
	static ThreadHolder gCurrentThread;
	static thread_id	gMainTID = Thread::currentTID();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class RunnableHolder : public Runnable
{
public:
	RunnableHolder(Runnable& target)
		: m_target(target)
	{
	}

	~RunnableHolder()
	{
	}

	void run()
	{
		m_target.run();
	}

private:
	Runnable& m_target;
};

class CallableHolder : public Runnable
{
public:
	CallableHolder(Thread::Callable callable, void* data) 
		: m_callable(callable)
		, m_data(data)
	{
	}

	~CallableHolder()
	{
	}

	void run()
	{
		m_callable(m_data);
	}

private:
	Thread::Callable m_callable;
	void* m_data;
};

//////////////////////////////////////////////////////////////////////////
// Thread static
//////////////////////////////////////////////////////////////////////////
void Thread::sleep(long milliseconds)
{
#ifdef _WIN32
	::Sleep(milliseconds);
#else
	::usleep(milliseconds * 1000);
#endif
}

void Thread::yield()
{
#ifdef _WIN32
	::Sleep(0);
#else
	sched_yield();
#endif
}

Thread* Thread::current()
{
	Thread* th = gCurrentThread.get();
	return th;
}

Thread::TID Thread::currentTID()
{
#ifdef _WIN32
	return ::GetCurrentThreadId();
#elif (CUTE_OS == CUTE_OS_MAC_OS_X)
    return pthread_mach_thread_np(pthread_self());
#elif defined(CUTE_OS_FAMILY_LINUX)
#ifndef SYS_gettid
#define SYS_gettid __NR_gettid
#endif
    return static_cast<Thread::TID>( syscall (SYS_gettid) );
#else
	return pthread_self();
#endif
}

bool Thread::isMainThread()
{
	return currentTID() == gMainTID;
}

int Thread::getMinOSPriority(int policy /* = POLICY_DEFAULT */)
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	return PRIO_LOWEST;
#else
#	if defined(CUTE_THREAD_PRIORITY_MIN)
	return CUTE_THREAD_PRIORITY_MIN;
#	elif defined(__VMS) || defined(__digital__)
	return PRI_OTHER_MIN;
#	else
	return sched_get_priority_min(policy);
#	endif
#endif
}

int Thread::getMaxOSPriority(int policy /* = POLICY_DEFAULT */)
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	return PRIO_HIGHEST;
#else
#	if defined(CUTE_THREAD_PRIORITY_MAX)
	return CUTE_THREAD_PRIORITY_MAX;
#	elif defined(__VMS) || defined(__digital__)
	return PRI_OTHER_MAX;
#	else
	return sched_get_priority_max(policy);
#	endif
#endif
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
String makeName(int id)
{
	std::ostringstream threadName;
	threadName << '#' << id;
	return threadName.str();
}

int uniqueId()
{
	static Atomic counter;
	return ++counter;
}

#ifdef CUTE_OS_FAMILY_POSIX
int mapPrio(int prio, int policy = SCHED_OTHER)
{
	int pmin = Thread::getMinOSPriority(policy);
	int pmax = Thread::getMaxOSPriority(policy);

	switch (prio)
	{
	case Thread::PRIO_LOWEST:
		return pmin;
	case Thread::PRIO_LOW:
		return pmin + (pmax - pmin) / 4;
	case Thread::PRIO_NORMAL:
		return pmin + (pmax - pmin) / 2;
	case Thread::PRIO_HIGH:
		return pmin + 3 * (pmax - pmin) / 4;
	case Thread::PRIO_HIGHEST:
		return pmax;
	default:
		cute_bugcheck_msg("invalid thread priority");
	}
	return -1; // just to satisfy compiler - we'll never get here anyway
}

int reverseMapPrio(int prio, int policy = SCHED_OTHER)
{
	if (policy == SCHED_OTHER)
	{
		int pmin = Thread::getMinOSPriority(policy);
		int pmax = Thread::getMaxOSPriority(policy);
		int normal = pmin + (pmax - pmin) / 2;
		if (prio == pmax)
			return Thread::PRIO_HIGHEST;
		if (prio > normal)
			return Thread::PRIO_HIGH;
		else if (prio == normal)
			return Thread::PRIO_NORMAL;
		else if (prio > pmin)
			return Thread::PRIO_LOW;
		else
			return Thread::PRIO_LOWEST;
	}
	else
	{
		return Thread::PRIO_HIGHEST;
	}
}
#endif

Thread::Thread()
	: m_target(0)
	, m_thread(0)
	, m_id(uniqueId())
	, m_prio(PRIO_NORMAL)
	, m_prioOS(0)
	, m_policy(SCHED_OTHER)
	, m_stackSize(0)
	, m_cpu(-1)
	, m_name(makeName(m_id))
	, m_pTLS(0)
{
}

Thread::Thread(const String& name)
	: m_target(0)
	, m_thread(0)
	, m_id(uniqueId())
	, m_prio(PRIO_NORMAL)
	, m_prioOS(0)
	, m_policy(SCHED_OTHER)
	, m_stackSize(0)
	, m_cpu(-1)
	, m_name(name)
	, m_pTLS(0)
{
}

Thread::~Thread()
{
	delete m_pTLS;
}

void Thread::startInternal(Runnable* target)
{
	if (isRunning())
		throw SystemException("thread already running");

	m_target = target;

#if defined(CUTE_OS_FAMILY_WINDOWS)
	//////////////////////////////////////////////////////////////////////////
	// windows
	//////////////////////////////////////////////////////////////////////////
#	ifdef _DLL
	m_thread = CreateThread(NULL, m_stackSize, &RunnableEntry, this, 0, &m_tid);
#	else
	unsigned threadId;
	m_thread = (HANDLE)_beginthreadex(NULL, m_stackSize, &RunnableEntry, this, 0, &threadId);
	m_tid = threadId;
#	endif

	if (!m_thread)
		throw SystemException("cannot create thread");

	if (m_prio != PRIO_NORMAL && !SetThreadPriority(m_thread, m_prio))
		throw SystemException("cannot set thread priority");

#else
	//////////////////////////////////////////////////////////////////////////
	// posix
	//////////////////////////////////////////////////////////////////////////
	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (m_stackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, m_stackSize))
		{
			pthread_attr_destroy(&attributes);
			throw SystemException("cannot set thread stack size");
		}
	}

	if (pthread_create(&m_thread, &attributes, &RunnableEntry, this))
	{
		pthread_attr_destroy(&attributes);
		throw SystemException("cannot start thread");
	}

#if CUTE_OS == CUTE_OS_LINUX
	// On Linux the TID is acquired from the running thread using syscall
	m_tid = 0;
#elif CUTE_OS == CUTE_OS_MAC_OS_X
	m_tid = static_cast<TID>(pthread_mach_thread_np(m_thread));
#else
	m_tid = m_thread;
#endif

	//_pData->started = true;
	pthread_attr_destroy(&attributes);

	if (m_policy == SCHED_OTHER)
	{
		if (m_prio != PRIO_NORMAL)
		{
			struct sched_param par;
			par.sched_priority = mapPrio(m_prio, SCHED_OTHER);
			if (pthread_setschedparam(m_thread, SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
		}
	}
	else
	{
		struct sched_param par;
		par.sched_priority = m_prioOS;
		if (pthread_setschedparam(m_thread, m_policy, &par))
			throw SystemException("cannot set thread priority");
	}

#endif
}

void Thread::run()
{
	if (m_target)
		m_target->run();
}

void Thread::start(Runnable& target)
{
	startInternal(new RunnableHolder(target));
}

void Thread::start(Callable target, void* data/* = 0 */)
{
	startInternal(new CallableHolder(target, data));
}

void Thread::join()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	switch (WaitForSingleObject(m_thread, INFINITE))
	{
	case WAIT_OBJECT_0:
		detach();
		return;
	default:
		throw SystemException("cannot join thread");
	}
#else
    m_done.wait();
	void* result;
	if (pthread_join(m_thread, &result))
		throw SystemException("cannot join thread");
#endif
}

bool Thread::join(long milliseconds)
{
	if (!isRunning())
		return true;

#if defined(CUTE_OS_FAMILY_WINDOWS)
	switch (WaitForSingleObject(m_thread, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		detach();
		return true;
	default:
		throw SystemException("cannot join thread");
	}
//#elif defined(__GLIBC__) || defined(__GNU_LIBRARY__)
//	// todo: pthread_timedjoin_np
//	void* result;
//	struct timespec abstime;
//	abstime.tv_sec = milliseconds / 1000;
//	abstime.tv_nsec = (milliseconds % 1000) * 1000000;
//	if (pthread_timedjoin(m_thread, &result, &abstime))
//		throw SystemException("cannot join thread");
#else
    void* result;
    if(m_done.tryWait(milliseconds))
    {
        if(pthread_join(m_thread, &result))
            throw SystemException("cannot join thread");
        return true;
    }
    else
    {
        return true;
    }
#endif
}

void Thread::detach()
{
	if (m_thread == 0)
		return;

#ifdef _WIN32
	if (CloseHandle(m_thread))
		m_thread = 0;
#else
	int ret = pthread_detach(m_thread);
	if (ret != 0)
		throw SystemException("pthread_detach");
	m_thread = NULL;
#endif
}

bool Thread::isRunning() const
{
	return m_thread != NULL;
}

ThreadLocalStorage& Thread::tls()
{
	if (!m_pTLS)
		m_pTLS = new ThreadLocalStorage;
	return *m_pTLS;
}

void Thread::clearTLS()
{
	if (m_pTLS)
	{
		delete m_pTLS;
		m_pTLS = 0;
	}
}

void Thread::setName(String& name)
{
	m_name = name;
}

void Thread::setStackSize(int size)
{
#if defined(CUTE_OS_FAMILY_UNIX) && defined(PTHREAD_STACK_MIN)
	if (size != 0)
	{
#if defined(CUTE_OS_FAMILY_BSD)
		// we must round up to a multiple of the memory page size
		const int STACK_PAGE_SIZE = 4096;
		size = ((size + STACK_PAGE_SIZE - 1) / STACK_PAGE_SIZE) * STACK_PAGE_SIZE;
#endif
		if (size < PTHREAD_STACK_MIN)
			size = PTHREAD_STACK_MIN;
	}
#endif

	m_stackSize = size;
}

void Thread::setAffinity(int cpu)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	DWORD mask = 1;
	mask <<= cpu;
	if (SetThreadAffinityMask(m_thread, mask) == 0)
		throw SystemException("Failed to set affinity");

	m_cpu = cpu;
#elif (CUTE_OS == CUTE_OS_MAC_OS_X)
	kern_return_t ret;
	thread_affinity_policy policy;
	policy.affinity_tag = cpu;

	ret = thread_policy_set(
		pthread_mach_thread_np(m_thread),
		THREAD_AFFINITY_POLICY,
		(thread_policy_t)&policy,
		THREAD_AFFINITY_POLICY_COUNT);

	if (ret != KERN_SUCCESS)
		throw SystemException("Failed to set affinity");

	yield();
#else

#	ifdef HAVE_PTHREAD_SETAFFINITY_NP
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
#	ifdef HAVE_THREE_PARAM_SCHED_SETAFFINITY
	if (pthread_setaffinity_np(m_thread, sizeof(cpuset), &cpuset) != 0)
		throw SystemException("Failed to set affinity");
#	else
	if (pthread_setaffinity_np(m_thread, &cpuset) != 0)
		throw SystemException("Failed to set affinity");
#	endif
#	endif
	yield();
#endif
}

void Thread::setPriority(Priority prio)
{
	if (m_prio != prio)
	{
		m_prio = prio;
		m_policy = SCHED_OTHER;
		if (isRunning())
		{
#ifdef CUTE_OS_FAMILY_WINDOWS
			if (SetThreadPriority(m_thread, m_prio) == 0)
				throw SystemException("cannot set thread priority");
#else
			struct sched_param par;
			par.sched_priority = mapPrio(m_prio, SCHED_OTHER);
			if (pthread_setschedparam(m_thread, SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
#endif
		}
	}
}

void Thread::setOSPriority(int prio, int policy /* = POLICY_DEFAULT */)
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	setPriority((Priority)prio);
#else
	if (m_prioOS != prio || m_policy != policy)
	{
		if (isRunning())
		{
			struct sched_param par;
			par.sched_priority = prio;
			if (pthread_setschedparam(m_thread, policy, &par))
				throw SystemException("cannot set thread priority");
		}

		m_prio = (Priority)reverseMapPrio(prio, policy);
		m_prioOS = prio;
		m_policy = policy;
	}
#endif
}

CUTE_NS_END
