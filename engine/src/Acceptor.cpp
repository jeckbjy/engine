#include "Acceptor.h"
#include "NetService.h"

CU_NS_BEGIN

Acceptor::Acceptor(IOService* service, uint type)
: AcceptChannel(service)
, m_type(type)
{

}

Acceptor::~Acceptor()
{

}

void Acceptor::completed(socket_t sock)
{
	AcceptEvent* ev = new AcceptEvent(this, sock);
	gNetService->post(ev);
}

CU_NS_END