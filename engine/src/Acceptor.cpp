#include "Acceptor.h"
#include "NetService.h"

CU_NS_BEGIN

struct AcceptEvent : public NetEvent
{
	Acceptor* m_acceptor;
	socket_t m_sock;
	AcceptEvent(Acceptor* acceptor, socket_t sock) :m_acceptor(acceptor), m_sock(sock){}
	bool process(NetService* server)
	{
		gNetService->onAccept(m_acceptor, m_sock);
		return true;
	}
};

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