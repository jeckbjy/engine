#pragma once
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

template<typename T>
class Singleton
{
public:
	static T& instance()
	{
		static T g_instance;
		return g_instance;
	}

protected:
	Singleton(void){};
	virtual ~Singleton(){};

private:
	Singleton(const Singleton&);
	Singleton& operator =(const Singleton&);
};

/// This is a helper template class for managing
/// singleton objects allocated on the heap.
/// The class ensures proper deletion (including
/// calling of the destructor) of singleton objects
/// when the application that created them terminates.
template <class S>
class SingletonHolder
{
public:
	SingletonHolder()
		: m_instance(0)
	{
	}

	~SingletonHolder()
	{
		delete m_instance;
	}

	S* get()
	{
		Mutex::ScopedLock lock(m_mutex);
		if (!m_instance) 
			m_instance = new S;
		return m_instance;
	}

	void reset()
	{
		Mutex::ScopedLock lock(m_mutex);
		delete m_instance;
		m_instance = 0;
	}

private:
	S*		m_instance;
	Mutex	m_mutex;
};

CUTE_NS_END