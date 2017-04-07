//! Server
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class PacketEvent;
class CUTE_CORE_API Handler
{
public:
	Handler() :m_next(NULL),m_id(0){}
	virtual ~Handler(){}
	virtual void handle(PacketEvent* ev) = 0;

	Handler* getNext() { return m_next; }
	void setNext(Handler* handler) { m_next = handler; }

	uint32 getID() const { return m_id; }
	void setID(uint32 id) { m_id = id; }

private:
	Handler*	m_next;
	uint32		m_id;
};

CUTE_NS_END
