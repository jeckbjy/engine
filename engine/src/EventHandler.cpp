//! Server
#include "Cute/EventHandler.h"
#include "Cute/HandlerService.h"
#include "Cute/HandlerProfiler.h"
#include "Cute/Packet.h"
#include "Cute/Server.h"
#include "Cute/SessionEvent.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
EventHandler::EventHandler()
	: m_async(false)
	, m_type(0)
{
}

void EventHandler::process(PacketEvent* ev)
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
