#include "WorkQueue.h"
#include "Util.h"

CU_NS_BEGIN

void WorkQueue::WorkThread(Thread* thread)
{
	WorkQueue* worker = (WorkQueue*)thread->data();
	worker->loop(thread->index());
}

WorkQueue::WorkQueue()
{

}

WorkQueue::~WorkQueue()
{
	for (ThreadPool::iterator itor = m_threads.begin(); itor != m_threads.end(); ++itor)
	{
		(*itor)->join();
		delete (*itor);
	}
	m_threads.clear();
}

void WorkQueue::create(int threads)
{
	m_init = 1;
	// 获取线程数目
	if (threads == -1)
		threads = Util::cpuCount() - 1;
	if (threads == 0)
		return;
	if (threads >= UINT8_MAX)
		threads = UINT8_MAX;

	for (int i = 1; i <= threads; ++i)
	{
		Thread* thread = new Thread(i);
		m_threads.push_back(thread);
		thread->start(&WorkThread, this);
	}
}

WorkItem* WorkQueue::newItem()
{
	Guard guard(m_spin);
	if (m_pool.empty())
		return new WorkItem();
	WorkItem* item = m_pool.front();
	m_pool.pop_front();
	return item;
}

void WorkQueue::freeItem(WorkItem* item)
{
	Guard guard(m_spin);
	if (m_pool.size() >= m_maxPool)
		delete item;
	else
		m_pool.push_back(item);
}

void WorkQueue::addItem(WorkItem* item, bool urgent)
{
	Guard guard(m_spin);
	if (urgent)
		m_urgent.push_back(item);
	else
		m_items.push_back(item);
}

void WorkQueue::process(void* param, uint32_t index)
{
	ItemList* items = (ItemList*)param;
	m_spin.lock();
	while (!items->empty())
	{
		WorkItem* item = items->front();
		items->pop_front();
		m_spin.unlock();
		item->func(item, index);
		m_spin.lock();
		m_pool.push_back(item);
	}
	m_spin.unlock();
}

void WorkQueue::execute(bool urgent_only)
{
	m_event.notify();
	process(&m_urgent, 0);
	if (!urgent_only)
		process(&m_items, 0);
	// 判断所有线程是否完成
	while (m_working > 0);
}

void WorkQueue::loop(size_t index)
{
	while (!m_quit)
	{
		m_event.wait();
		++m_working;
		process(&m_urgent, index);
		--m_working;
		process(&m_items, index);
	}
}

CU_NS_END