#pragma once
#include "Cute/IOLoop.h"
#include "Cute/Thread.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

class CUTE_CORE_API IOLoopGroup
{
public:
	IOLoopGroup();
	~IOLoopGroup();

	void run(size_t count);
	void stop();

	IOLoop* main();
	IOLoop* next(bool ignoreMain = true);

private:
	typedef std::vector<IOLoop*> IOLoopVec;
	typedef std::vector<Thread*> ThreadVec;

	bool		m_stopped;
	IOLoopVec	m_loops;
	ThreadVec	m_threads;
	size_t		m_index;
	Mutex		m_mutex;
};

CUTE_NS_END
