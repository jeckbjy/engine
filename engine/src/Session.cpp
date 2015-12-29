#include "Session.h"
#include "NetEvent.h"
#include "NetService.h"

CU_NS_BEGIN

Session::Session(uint id, IOService* service, IProtocol* proto, socket_t sock)
: m_id(id)
, m_type(0)
, m_proto(proto)
, m_data(0)
{
	m_channel = new SocketChannel(&Session::notify,service, sock);
	m_channel->setCallbackOwner(this);
	m_channel->retain();
}

Session::~Session()
{
	m_channel->reset();
	m_channel->release();
}

void Session::send(const char* str)
{
	Buffer buf;
	buf.write(str, strlen(str));
	send(buf);
}

void Session::notify(uint8_t type)
{
	switch (type)
	{
	case SocketChannel::EV_ERROR:
	{
		ErrorEvent* ev = new ErrorEvent();
		ev->sess = this;
		ev->code = m_channel->getLastCode();
		gNetService->post(ev);
		break;
	}
	case SocketChannel::EV_CONNECT:
	{
		ConnectEvent* ev = new ConnectEvent();
		ev->sess = this;
		gNetService->post(ev);
		break;
	}
	case SocketChannel::EV_READ:
	{
		if (m_proto)
			m_proto->process(this);
		break;
	}
	}
}

CU_NS_END
