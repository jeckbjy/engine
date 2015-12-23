#include "Acceptor.h"
#include "ServerEvent.h"
#include "Server.h"

CU_NS_BEGIN

struct AcceptEvent : public ServerEvent
{
	Acceptor* m_acceptor;
	socket_t m_sock;
	AcceptEvent(Acceptor* acceptor, socket_t sock) :m_acceptor(acceptor), m_sock(sock){}
	bool process(Server* server)
	{
		gServer->onAccept(m_acceptor, m_sock);
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
	gServer->post(ev);
}

CU_NS_END