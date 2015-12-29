#include "Acceptor.h"
#include "NetService.h"

CU_NS_BEGIN

Acceptor::Acceptor(IOService* service, uint type)
: m_type(type)
{
	m_channel = new AcceptChannel(&Acceptor::completed,service);
	m_channel->setCallbackOwner(this);
	m_channel->retain();
}

Acceptor::~Acceptor()
{
	m_channel->reset();
	m_channel->release();
}

void Acceptor::completed(error_t ec, socket_t sock)
{
	AcceptEvent* ev = new AcceptEvent(this, sock);
	gNetService->post(ev);
}

CU_NS_END