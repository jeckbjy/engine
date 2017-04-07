//! Network/Sockets
#include "Cute/SocketChannel.h"

CUTE_NS_BEGIN

SocketChannel::SocketChannel(IOLoop* loop)
	: IOChannel(loop)
	, m_listener(NULL)
{

}

SocketChannel::~SocketChannel()
{

}

void SocketChannel::setListener(SocketListener* listener)
{
	m_listener = listener;
}

CUTE_NS_END
