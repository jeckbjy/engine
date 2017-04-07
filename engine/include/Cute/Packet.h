#pragma once
#include "Cute/Foundation.h"
#include "Cute/Message.h"
#include "Cute/Ref.h"

CUTE_NS_BEGIN

class Packet : public Message, public Ref<Packet>
{
public:
	Packet() :m_parent(NULL){}
	virtual ~Packet() {}
	virtual uint32 msgID() const = 0;

protected:
	Packet* m_parent;
	Object* m_owner;	// session ??
	//Buffer	m_head;
};

// 服务器间中转消息
class ServerPacket : public Packet
{
public:
protected:
	uint64		m_uid;
	Message*	m_message;
};

CUTE_NS_END
