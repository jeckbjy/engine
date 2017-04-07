//! IO
#include "Cute/IOLoopGroup.h"
#include "Cute/Environment.h"

CUTE_NS_BEGIN

IOLoopGroup::IOLoopGroup()
	: m_stopped(true)
	, m_index(0)
{
}

IOLoopGroup::~IOLoopGroup()
{
	stop();
}

void IOLoopGroup::run(size_t count)
{
	Mutex::ScopedLock lock(m_mutex);

	if (count == 0)
		count = Environment::processorCount();

	for (size_t i = 0; i < count; ++i)
	{
		IOLoop* looper = new IOLoop();
		m_loops.push_back(looper);
		Thread* thd = new Thread();
		m_threads.push_back(thd);
		thd->start(*looper);
	}

	m_stopped = false;
}

void IOLoopGroup::stop()
{
	Mutex::ScopedLock lock(m_mutex);

	if (m_stopped)
		return;

	m_stopped = true;

	// stop
	for (size_t i = 0; i < m_loops.size(); ++i)
	{
		m_loops[i]->stop();
	}

	for (size_t i = 0; i < m_threads.size(); ++i)
	{
		m_threads[i]->join();
	}

	// free
	for (size_t i = 0; i < m_loops.size(); ++i)
	{
		delete m_loops[i];
	}

	for (size_t i = 0; i < m_threads.size(); ++i)
	{
		delete m_threads[i];
	}

	m_loops.clear();
	m_threads.clear();
}

IOLoop* IOLoopGroup::main()
{
	Mutex::ScopedLock lock(m_mutex);

	if (m_stopped || m_loops.empty())
		return NULL;

	return m_loops[0];
}

IOLoop* IOLoopGroup::next(bool ignoreMain)
{
	Mutex::ScopedLock lock(m_mutex);

	if (m_stopped || m_loops.empty())
		return NULL;

	if (m_loops.size() == 1)
	{
		return m_loops[0];
	}
	else
	{
		size_t index = m_index;
		if (ignoreMain && (index == 0))
			index = 1;
		m_index = (index + 1) % m_loops.size();

		return m_loops[index];
	}
}

CUTE_NS_END
