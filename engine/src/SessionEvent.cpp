//! Server
#include "Cute/SessionEvent.h"
#include "Cute/Packet.h"
#include "Cute/Session.h"

CUTE_NS_BEGIN

SessionEvent::SessionEvent(Session* sess)
	: m_session(sess)
{
}

SessionEvent::~SessionEvent()
{
}

bool SessionEvent::isValid() const
{
	return m_session->isActive() && !m_session->isClosing();
}

PacketEvent::PacketEvent(Session* sess, Packet* packet)
	: SessionEvent(sess)
	, m_packet(packet)
{
}

PacketEvent::~PacketEvent()
{
}

void PacketEvent::process()
{
	//uint32 msgid = 0;
	//SocketHandler* handler = HandlerService::get().find(msgid); 
	//if (handler != NULL)
	//{
	//	handler->process(*m_packet, m_socket);
	//}
}

CUTE_NS_END
