#pragma once
#include "API.h"

CU_NS_BEGIN

#ifdef _WIN32
typedef HANDLE		thread_t;
typedef unsigned	thread_id;
//typedef DWORD		thread_id;
#else
typedef pthread_t	thread_t;
typedef pthread_t	thread_id;
#endif

class Thread;
class CU_API Runnable : public Ref
{
public:
	virtual ~Runnable(){}
	virtual void run() = 0;
};

// Ӧ��ʹ��_beginthreadex����CreateThread
// http://blog.csdn.net/shines/article/details/7055510
// http://blog.sina.com.cn/s/blog_9635e5ef0101psgk.html
class CU_API Thread
{
public:
	enum Priority
	{
		Lowest,
		Low,
		Normal,
		High,
		Hightest,
	};

	typedef void(*func_t)(void* args);
	
	static void sleep(long msec);	// ���ߺ���
	static void yield();			// �ó�cpu
	static Thread*	 current();
	static thread_id currentID();
	static bool	isMainThread();

public:
	Thread(size_t size = 0);
	~Thread();

	void start(Runnable* target);
	void start(func_t fun, void* param = 0);
	void join();
	void detach();
	bool joinable()const { return m_handle != NULL; }
	bool running() const { return m_handle != NULL; }

	thread_id id() const { return m_id; }
	void* data() const { return m_data; }

public:
	void process();	// ���ڲ�����

private:
	// noncopyable
	Thread(const Thread&);
	Thread& operator=(const Thread&);

	thread_t	m_handle;
	thread_id	m_id;
	size_t		m_size;		// ��ջ��С
	func_t		m_func;
	void*		m_data;
};

CU_NS_END