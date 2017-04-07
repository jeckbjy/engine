#pragma once
#include "Cute/Foundation.h"
#include "Cute/Mutex.h"
#include "Cute/Event.h"

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOWS)
typedef HANDLE			thread_t;
#else
typedef pthread_t		thread_t;
#endif

#if defined(CUTE_OS_FAMILY_WINDOWS)
typedef DWORD			thread_id;
#elif (CUTE_OS == CUTE_OS_LINUX)
typedef pid_t			thread_id;
#elif (CUTE_OS == CUTE_OS_MAC_OS_X)
typedef mach_port_t		thread_id;
#else
typedef pthread_t		thread_id;
#endif

#if defined(CUTE_OS_FAMILY_WINDOWS)
#	if defined(_DLL)
#	define __RET		DWORD
#	define __API		WINAPI
#	else
#	define __RET		unsigned
#	define __API		__stdcall
#	endif
#else
#	define __RET		void*
#	define __API
#endif

#ifdef CUTE_OS_FAMILY_UNIX
#	define THREAD_PRIORITY_LOWEST			0
#	define THREAD_PRIORITY_BELOW_NORMAL		1
#	define THREAD_PRIORITY_NORMAL			2
#	define THREAD_PRIORITY_ABOVE_NORMAL		3
#	define THREAD_PRIORITY_HIGHEST			4
#endif

#ifndef SCHED_OTHER
#define SCHED_OTHER 0
#endif

class Runnable;
class ThreadLocalStorage;

/// This class implements a platform-independent
/// wrapper to an operating system thread.
///
/// Every Thread object gets a unique (within
/// its process) numeric thread ID.
/// Furthermore, a thread can be assigned a name.
/// The name of a thread can be changed at any time.
class CUTE_CORE_API Thread
{
public:
	typedef thread_id TID;
	typedef void(*Callable)(void*);

	enum Priority
	{
		PRIO_LOWEST  = THREAD_PRIORITY_LOWEST,			/// The lowest thread priority.
		PRIO_LOW	 = THREAD_PRIORITY_BELOW_NORMAL,	/// A lower than normal thread priority.
		PRIO_NORMAL  = THREAD_PRIORITY_NORMAL,			/// The normal thread priority.
		PRIO_HIGH	 = THREAD_PRIORITY_ABOVE_NORMAL,	/// A higher than normal thread priority.
		PRIO_HIGHEST = THREAD_PRIORITY_HIGHEST,			/// The highest thread priority.
	};

	enum Policy
	{
		POLICY_DEFAULT = SCHED_OTHER
	};

	static void		sleep(long milliseconds);
	static void		yield();
	static Thread*	current();
	static TID		currentTID();
	static bool		isMainThread();

	static int		getMinOSPriority(int policy = POLICY_DEFAULT);
	static int		getMaxOSPriority(int policy = POLICY_DEFAULT);
public:
	Thread();
	Thread(const String& threadName);
	~Thread();

	void start(Runnable& target);
	void start(Callable target, void* data = 0);

	void join();
	bool join(long milliseconds);
	void detach();

	bool isRunning() const;

	int id() const;
	TID tid() const;

	String name() const;
	String getName() const;

	int getStackSize() const;
	int getAffinity() const;
	int getOSPriority() const;
	Priority getPriority() const;

	void setName(String& name);
	void setStackSize(int size);
	void setAffinity(int cpu);
	void setPriority(Priority prio);
	void setOSPriority(int prio, int policy = POLICY_DEFAULT);

protected:
	ThreadLocalStorage& tls();
	void clearTLS();

	void startInternal(Runnable* target);
	void run();

private:
	Thread(const Thread&);
	Thread& operator = (const Thread&);
	friend __RET __API RunnableEntry(void*);

	Runnable*			m_target;
	thread_t			m_thread;
	TID					m_tid;
	int                 m_id;
	Priority			m_prio;
	int					m_prioOS;
	int					m_policy;
	int					m_stackSize;
	int					m_cpu;
	String		        m_name;
	ThreadLocalStorage* m_pTLS;

	friend class ThreadLocalStorage;
	friend class PooledThread;
};

//
// inlines
//
inline Thread::TID Thread::tid() const
{
	return m_tid;
}

inline int Thread::id() const
{
	return m_id;
}

inline String Thread::name() const
{
	return m_name;
}

inline String Thread::getName() const
{
	return m_name;
}

inline int Thread::getStackSize() const
{
	return m_stackSize;
}

inline int Thread::getAffinity() const
{
	return m_cpu;
}

inline Thread::Priority Thread::getPriority() const
{
	return m_prio;
}

inline int Thread::getOSPriority() const
{
	return m_prioOS;
}

CUTE_NS_END