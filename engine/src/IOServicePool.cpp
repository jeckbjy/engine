#include "IOServicePool.h"

CU_NS_BEGIN

IOServicePool::IOServicePool()
: m_stopped(true)
, m_index(0)
{

}

IOServicePool::~IOServicePool()
{
	stop();
}

void IOServicePool::run(size_t services /* = 1 */, size_t workers /* = 1 */)
{
	if (services < 1)
		services = 1;
	for (size_t i = 0; i < services; ++i)
	{
		IOService* service = new IOService();
		m_services.push_back(service);
		for (size_t j = 0; j < workers; ++j)
		{
			Thread* thd = new Thread();
			m_threads.push_back(thd);
			thd->start(&IOService::WorkThread, service);
		}
	}
	m_stopped = false;
}

void IOServicePool::stop()
{
	if (m_stopped)
		return;
	m_stopped = true;
	for (ServiceVec::iterator itor = m_services.begin(); itor != m_services.end(); ++itor)
	{
		(*itor)->stop();
		delete (*itor);
	}
	for (ThreadVec::iterator itor = m_threads.begin(); itor != m_threads.end(); ++itor)
	{
		(*itor)->join();
		delete (*itor);
	}
	m_services.clear();
	m_threads.clear();
}

IOService* IOServicePool::next()
{
	if (m_stopped)
		return 0;
	IOService* service = m_services[m_index];
	m_index = (m_index + 1) % m_services.size();
	return service;
}

CU_NS_END
