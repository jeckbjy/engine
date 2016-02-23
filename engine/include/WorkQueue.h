#pragma once
#include "API.h"
#include "List.h"
#include "Mutex.h"
#include "Thread.h"
#include "Atomic.h"
#include "SyncEvent.h"
#include "Singleton.h"

CU_NS_BEGIN

struct WorkItem : public ListNode<WorkItem>
{
	// thread index (0=main thread)
	typedef void(*func_t)(const WorkItem*, unsigned);
public:
	WorkItem():start(0),end(0),data(0){}
	void*	start;
	void*	end;
	void*	data;
	func_t	func;
};

// 同步多线程加速运算
class CU_API WorkQueue : public Singleton<WorkQueue>
{
	friend class WorkThread;
public:
	WorkQueue();
	~WorkQueue();

	void execute(bool urgent_only = true);
	void create(int threads = -1);
	void freeItem(WorkItem* item);
	void addItem(WorkItem* item, bool urgent);
	WorkItem* newItem();

private:
	void loop(size_t index);
	void process(void* param, uint32_t index);

private:
	typedef List<WorkItem>		ItemList;
	typedef LockGuard<SpinLock>	Guard;
	typedef Vector<Thread*>		ThreadPool;

	uint8_t		m_quit : 1;
	uint8_t		m_init : 1;

	ThreadPool	m_threads;
	ItemList	m_pool;
	ItemList	m_items;
	ItemList	m_urgent;	// 必须执行完才能执行主线程
	size_t		m_maxPool;
	Atomic		m_working;
	SpinLock	m_spin;
	SyncEvent	m_event;
};

CU_NS_END
