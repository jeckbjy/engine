// module Thread
#include "Cute/ThreadLocal.h"
#include "Cute/Thread.h"
#include "Cute/Singleton.h"

CUTE_NS_BEGIN

TLSAbstractSlot::TLSAbstractSlot()
{
}

TLSAbstractSlot::~TLSAbstractSlot()
{
}

ThreadLocalStorage::ThreadLocalStorage()
{
}

ThreadLocalStorage::~ThreadLocalStorage()
{
	for (TLSMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
	{
		delete it->second;
	}
}

TLSAbstractSlot*& ThreadLocalStorage::get(const void* key)
{
	TLSMap::iterator it = m_map.find(key);
	if (it == m_map.end())
		return m_map.insert(TLSMap::value_type(key, reinterpret_cast<TLSAbstractSlot*>(0))).first->second;
	else
		return it->second;
}

namespace
{
	static SingletonHolder<ThreadLocalStorage> sh;
}

ThreadLocalStorage& ThreadLocalStorage::current()
{
	Thread* pThread = Thread::current();
	if (pThread)
	{
		return pThread->tls();
	}
	else
	{
		return *sh.get();
	}
}

void ThreadLocalStorage::clear()
{
	Thread* pThread = Thread::current();
	if (pThread)
		pThread->clearTLS();
}

CUTE_NS_END