//! Server
#include "Cute/HandlerProvider.h"
#include "Cute/HandlerService.h"

CUTE_NS_BEGIN

HandlerProvider::HandlerProvider()
	: m_head(NULL)
{
}

HandlerProvider::~HandlerProvider()
{
	cleanup();
}

void HandlerProvider::regist(uint msgid, Handler* handler)
{
	HandlerService::get().regist(msgid, handler);
	handler->setNext(m_head);
	m_head = handler;
}

void HandlerProvider::cleanup()
{
	// ·´×¢²á
	Handler* handler = m_head;
	while (handler != NULL)
	{
		HandlerService::get().remove(handler->getID());
		handler = handler->getNext();
	}
}

CUTE_NS_END
