#include "Thread.h"
#ifdef _WIN32
#include <process.h>	// _beginthreadex
#endif

CU_NS_BEGIN

#ifdef _WIN32
#	define __RET				unsigned
#	define __API				__stdcall
//#	define __RET				DWORD
//#	define __API				WINAPI

#	define __key_t				DWORD
#	define __key_create(key)	(key = TlsAlloc()) == TLS_OUT_OF_INDEXES
#	define __key_free(key)		TlsFree(key)
#	define __key_get(key)		TlsGetValue(key)
#	define __key_set(k,v)		TlsSetValue(k,v)
#else
#	define __RET				void*
#	define __API
#	define __key_t				pthread_key_t
#	define __key_create(key)	pthread_key_create(&key, NULL)
#	define __key_free(key)		pthread_key_delete(key)
#	define __key_get(key)		pthread_getspecific(key)
#	define __key_set(k,v)		pthread_setspecific(k,v)
#endif

class ThreadHolder
{
	__key_t _key;
public:
	ThreadHolder()
	{
		if (__key_create(_key))
			throw std::runtime_error("cannot allocate thread context key");
	}
	~ThreadHolder(){ __key_free(_key); }
	Thread* get() const{ return reinterpret_cast<Thread*>(__key_get(_key)); }
	void set(Thread* data) { __key_set(_key, data); }
};

static ThreadHolder gCurThread;
static thread_id	gMainThreadID = Thread::currentID();

static __RET __API Entry(void* arg)
{
	Thread* impl = reinterpret_cast<Thread*>(arg);
	gCurThread.set(impl);
	impl->process();
	return 0;
}

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

thread_id Thread::currentID()
{
#ifdef _WIN32
	return ::GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

bool Thread::isMainThread()
{
	return currentID() == gMainThreadID;
}

Thread* Thread::current()
{
	Thread* thread = gCurThread.get();
	return thread;
}

Thread::Thread(size_t index, size_t size)
: m_handle(0)
, m_id(0)
, m_index(index)
, m_size(size)
, m_func(0)
, m_data(0)
{

}

Thread::~Thread()
{
	detach();
}

void RunnableEntry(Thread* thread)
{
	Runnable* runner = (Runnable*)thread->data();
	runner->run(thread);
}

void Thread::start(Runnable* target)
{
	this->start(&RunnableEntry, 0);
}

void Thread::start(func_t func, void* data /* = 0 */)
{
	if (joinable())
		throw std::runtime_error("thread already running");

	m_func = func;
	m_data = data;
#ifdef _WIN32
	m_handle = (thread_t)_beginthreadex(NULL, m_size, &Entry, this, 0, &m_id);
	//_thread = CreateThread(NULL, _stackSize, &Entry, this, 0, &_thread_id);
	if (m_handle == NULL)
		throw std::runtime_error("cannot create thread");
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (m_size != 0)
	{
		if (pthread_attr_setstacksize(&attr, m_size) != 0)
		{
			pthread_attr_destroy(&attr);
			throw std::runtime_error("cannot set thread stack size");
		}
	}
	if (pthread_create(&m_handle, &attr, &Entry, data) != 0)
	{
		m_handle = 0;
		pthread_attr_destroy(&attr);
		throw std::runtime_error("cannot start thread");
	}
	pthread_attr_destroy(&attr);
#endif
}

void Thread::join()
{
	if (!joinable())
		return;
#ifdef _WIN32
	switch (WaitForSingleObject(m_handle, INFINITE))
	{
	case WAIT_OBJECT_0:
		detach();
		return;
	default:
		throw std::exception("cannot join thread");
	}
#else
	void* result;
	if (pthread_join(m_handle, &result))
		throw std::runtime_error("cannot join thread");
#endif
}

void Thread::detach()
{
	if (m_handle == 0)
		return;
#ifdef _WIN32
	CloseHandle(m_handle);
#else
	int ret = pthread_detach(m_handle);
	if (ret != 0)
		throw std::runtime_error("pthread_detach");
#endif
	m_handle = NULL;
	m_id = 0;
}

void Thread::process()
{
#ifdef	CU_OS_POSIX
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif
	m_func(this);
	m_func = 0;
	m_data = 0;
}

CU_NS_END
