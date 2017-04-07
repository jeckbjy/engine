#pragma once
#include "Cute/LogicEvent.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

class Packet;
class Session;
class CUTE_CORE_API SessionEvent : public LogicEvent
{
public:
	SessionEvent(Session* sess);
	virtual ~SessionEvent();

	bool isValid() const;

	Session* getSession(){ return m_session; }

private:
	Session* m_session;
};

class CUTE_CORE_API PacketEvent : public SessionEvent
{
public:
	PacketEvent(Session* sess, Packet* packet);
	~PacketEvent();

	void process();

	Packet* getPacket() { return m_packet.get(); }

private:
	typedef RefPtr<Packet> PacketPtr;
	PacketPtr m_packet;
};

CUTE_NS_END
