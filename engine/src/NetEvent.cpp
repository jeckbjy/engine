#include "NetEvent.h"
#include "NetService.h"

CU_NS_BEGIN

bool ConnectEvent::process(NetService* serivce)
{
	serivce->onConnect(sess);
	return true;
}

bool ErrorEvent::process(NetService* serivce)
{
	serivce->onError(sess, code);
	return true;
}


bool PacketEvent::process(NetService* service)
{
	return service->onPacket(sess, msg);
}

CU_NS_END