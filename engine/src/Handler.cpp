//! Server
#include "Cute/Handler.h"
#include "Cute/HandlerService.h"
#include "Cute/HandlerProfiler.h"
#include "Cute/Packet.h"
#include "Cute/Server.h"

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
	Handler* handler = HandlerService::instance().find(m_msg->msgid());
	if (handler)
		handler->process(this);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Handler::Handler()
	: m_async(false)
	, m_type(0)
{
}

void Handler::process(PacketEvent* ev)
{
	HandlerProfiler::Timer timer;
	HandlerProfiler::instance().start(timer);

	int cmdid = ev->getPacket()->msgid();
	int result = handle(ev);
	if (result != 0)
	{// 返回值处理
		if (result < 0)
		{
			Server::get().post(ev, 1);
		}
		else
		{// 报错预警

		}
	}

	HandlerProfiler::instance().hit(timer, cmdid);
}

CUTE_NS_END
