//! Server
#include "Cute/SessionEvent.h"
#include "Cute/Server.h"
#include "Cute/HandlerService.h"

CUTE_NS_BEGIN

PacketEvent::PacketEvent(Session* sess, Packet* msg)
	: m_sess(sess)
	, m_msg(msg)
{
}

PacketEvent::~PacketEvent()
{
}

void PacketEvent::process()
{
	EventHandler* handler = HandlerService::instance().find(m_msg->msgid());
	if (handler)
		handler->process(this);
}

TransferEvent::TransferEvent(Session* sess)
	: m_sess(sess)
{
}

void TransferEvent::process()
{
	Server::get().onTransfer(m_sess, &m_msg);
}

TextEvent::TextEvent(Session* sess, String& text)
	: m_sess(sess)
	, m_text(text)
{
}

void TextEvent::process()
{
	Server::get().onText(m_sess, m_text);
}

CUTE_NS_END
