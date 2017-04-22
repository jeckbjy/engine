//! Server
#include "Cute/HandlerService.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

HandlerService& HandlerService::instance()
{
	static HandlerService service;
	return service;
}

HandlerService::HandlerService()
{
}

HandlerService::~HandlerService()
{
	for (size_t i = 0; i < m_handlerVec.size(); ++i)
	{
		delete m_handlerVec[i];
	}

	for (HandlerMap::iterator itor = m_handlerMap.begin(); itor != m_handlerMap.end(); ++itor)
	{
		delete itor->second;
	}

	m_handlerVec.clear();
	m_handlerMap.clear();
}

EventHandler* HandlerService::find(uint32 msgid) const
{
	if (msgid < m_handlerVec.size())
	{
		return m_handlerVec[msgid];
	}
	else
	{
		HandlerMap::const_iterator itor = m_handlerMap.find(msgid);
		if (itor != m_handlerMap.end())
			return itor->second;

		return NULL;
	}
}

bool HandlerService::has(uint32 msgid) const
{
	return find(msgid) != NULL;
}

void HandlerService::regist(uint32 msgid, EventHandler* handler)
{
	if (has(msgid))
	{
		delete handler;
		throw InvalidArgumentException("duplicate msg:" + msgid);
	}

	//handler->setID(msgid);
	if (msgid >= HANDLER_MAX)
	{
		m_handlerMap[msgid] = handler;
	}
	else
	{
		if (msgid >= m_handlerVec.size())
			m_handlerVec.resize(msgid + 1);

		m_handlerVec[msgid] = handler;
	}
}

void HandlerService::remove(uint32 msgid)
{
	EventHandler* handler = find(msgid);
	if (handler == NULL)
		return;

	if (msgid >= HANDLER_MAX)
	{
		m_handlerMap.erase(msgid);
	}
	else
	{
		if (msgid < m_handlerVec.size())
			m_handlerVec[msgid] = NULL;
	}

	delete handler;
}

CUTE_NS_END
