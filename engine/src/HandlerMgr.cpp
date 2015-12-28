#include "HandlerMgr.h"

CU_NS_BEGIN

HandlerMgr& HandlerMgr::instance()
{
	static HandlerMgr mgr;
	return mgr;
}

HandlerMgr::HandlerMgr()
{

}

HandlerMgr::~HandlerMgr()
{
	for (HandlerVec::iterator itor = m_handlers.begin(); itor != m_handlers.end(); ++itor)
	{
		IHandler* handler = *itor;
		if (handler)
			delete handler;
	}
	m_handlers.clear();
}

IHandler* HandlerMgr::find(uint msgid)
{
	if (msgid < m_handlers.size())
		return m_handlers[msgid];
	return 0;
}

void HandlerMgr::add_handler(uint msgid, IHandler* handler)
{
	if (msgid >= m_handlers.size())
		m_handlers.resize(msgid + 1);
	assert(m_handlers[msgid] == 0);
	m_handlers[msgid] = handler;
}

CU_NS_END