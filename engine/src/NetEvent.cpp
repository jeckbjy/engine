#include "NetEvent.h"
#include "NetService.h"
#include "HandlerMgr.h"
#include "Log.h"
#include "Packet.h"

CU_NS_BEGIN

bool NetEvent::process(NetService* service)
{
	return service->onEvent(this);
}

bool AcceptEvent::process(NetService* service)
{
	service->onAccept(m_acceptor, m_sock);
	return true;
}

bool ConnectEvent::process(NetService* service)
{
	service->onConnect(sess);
	return true;
}

bool ErrorEvent::process(NetService* service)
{
	service->onError(sess, code);
	return true;
}

PacketEvent::~PacketEvent()
{
	if (msg)
		delete msg;
}

bool PacketEvent::process(NetService* service)
{
	int hr = handler->process(sess, msg);
	// for log
	if (hr != HRET_OK && hr != HRET_WAIT)
	{
		LOG_ERROR("process fail:msgid=%d", msg->msgid());
	}
	// return can free
	return hr != HRET_WAIT;
}

CU_NS_END