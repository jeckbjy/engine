//! Network/Sockets
#include "Cute/ServerChannel.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

ServerChannel::ServerChannel(IOLoop* loop)
	: IOChannel(loop)
	, m_listener(NULL)
	, m_id(0)
	, m_type(0)
{
}

ServerChannel::~ServerChannel()
{
}

void ServerChannel::setListener(ServerListener* listener)
{
	m_listener = listener;
}

CUTE_NS_END
