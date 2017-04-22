#pragma once
#include "Cute/Packet.h"
#include "Cute/EventBase.h"

CUTE_NS_BEGIN

class Packet;
class Session;
class CUTE_CORE_API PacketEvent : public EventBase
{
public:
	PacketEvent(Session* sess, Packet* msg);
	~PacketEvent();

	void process();

	Session* getSession() { return m_sess; }
	Packet*	 getPacket() { return m_msg; }

protected:
	Session* m_sess;
	Packet*	 m_msg;
};

class CUTE_CORE_API TransferEvent : public EventBase
{
public:
	TransferEvent(Session* sess);

	void process();

	Session*		getSession() { return m_sess; }
	TransferPacket* getPacket() { return &m_msg; }

protected:
	Session* m_sess;
	TransferPacket m_msg;
};

class CUTE_CORE_API TextEvent : public EventBase
{
public:
	TextEvent(Session* sess, String& text);

	void process();
protected:
	Session* m_sess;
	String	 m_text;
};

CUTE_NS_END
