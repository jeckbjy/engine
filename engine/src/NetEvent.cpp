#include "NetEvent.h"
#include "NetService.h"

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

bool PacketEvent::process(NetService* service)
{
	return service->onPacket(sess, msg);
}

CU_NS_END