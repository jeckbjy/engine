#include "Object.h"
#include "Context.h"

CU_NS_BEGIN

Object::~Object()
{
	Context::Instance().removeListener(this);
	// todo:ʹ��ͨ���㷨ʵ��
	//m_handlers.release();
	EventHandler* handler;
	HandlerList::iterator tmp;
	// ������ɾ��
	for (HandlerList::iterator itor = m_handlers.begin(); itor != m_handlers.end(); )
	{
		handler = *itor;
		Context::Instance().removeListener(*itor);
		tmp = itor++;
		m_handlers.erase(tmp);
		handler->release();
	}
}

const AttributeList& Object::getAttributes() const
{
	if (!getRtti()->hasAttributes())
		addAttributes();
	return getRtti()->getAttributes();
}

void Object::addListener(EventHandler* handler)
{
	handler->retain();
	m_handlers.push_back(handler);
	Context::Instance().addListener(handler);
}

void Object::removeListener(EventHandler* handler)
{
	HandlerList::iterator itor = m_handlers.begin();
	HandlerList::iterator endIt = m_handlers.end();
	for (; itor != endIt; ++itor)
	{
		if (*itor == handler)
		{
			m_handlers.erase(itor);
			handler->release();
			break;
		}
	}
}

void Object::removeListener(uint32_t type, Object* sender /* = NULL */)
{
	// ���Ҳ�ɾ��
	HandlerList::iterator itor = m_handlers.begin();
	HandlerList::iterator endIt = m_handlers.end();
	for (; itor != endIt; ++itor)
	{
		if (itor->equal(type, sender))
		{
			Context::Instance().removeListener(*itor);
			m_handlers.erase(itor);
			itor->release();
			break;
		}
	}
}

CU_NS_END