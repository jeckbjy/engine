#include "Session.h"
#include "NetEvent.h"
#include "NetService.h"

CU_NS_BEGIN

Session::Session(IOService* service, uint id, uint type, IProtocol* proto)
: SocketChannel(service)
, m_id(id)
, m_type(type)
, m_proto(proto)
, m_data(0)
{

}

Session::~Session()
{

}

void Session::notify(uint8_t type)
{
	switch (type)
	{
	case EV_ERROR:
	{
		ErrorEvent* ev = new ErrorEvent();
		ev->sess = this;
		ev->code = m_code;
		gNetService->post(ev);
		break;
	}
	case EV_CONNECT:
	{
		ConnectEvent* ev = new ConnectEvent();
		ev->sess = this;
		gNetService->post(ev);
		break;
	}
	case EV_READ:
	{
		if (m_proto)
			m_proto->process(this);
		break;
	}
	}
}

CU_NS_END