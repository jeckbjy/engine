#pragma once
#include "IOService.h"
#include "Thread.h"

CU_NS_BEGIN

// 至少有1个
class CU_API IOServicePool
{
public:
	IOServicePool();
	~IOServicePool();
	void run(size_t services = 1, size_t workers = 1);
	void stop();
	IOService* next();

private:
	typedef std::vector<IOService*> ServiceVec;
	typedef std::vector<Thread*>	ThreadVec;
	bool		m_stopped;
	ServiceVec	m_services;
	ThreadVec	m_threads;
	size_t		m_index;
};

CU_NS_END