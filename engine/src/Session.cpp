//! Server
#include "Cute/Session.h"
#include "Cute/Server.h"

CUTE_NS_BEGIN

Session::Session(SocketChannel* socket, uint32 id, uint32 type)
	: m_socket(socket)
	, m_id(id)
	, m_type(type)
	, m_data(NULL)
{
	if (m_socket)
	{
		m_socket->setListener(this);
		if (m_socket->isActive())
		{
			m_peer = m_socket->peerAddress();
		}
	}
}

Session::~Session()
{
}

bool Session::isActive() const
{
	return m_socket->isActive();
}

bool Session::isClosing() const
{
	return m_socket->isClosing();
}

void Session::setClosing()
{
	m_socket->setClosing(true);
}

void Session::send(const BufferList& data)
{
	m_socket->send(data);
}

void Session::connect(const SocketAddress& addr)
{
	m_peer = addr;
	m_socket->connect(m_peer);
}

void Session::reconnect()
{
	m_socket->connect(m_peer);
}

void Session::shutdown(int how /* = SHUT_RD */)
{
	m_socket->shutdown(how);
}

void Session::close()
{
	m_socket->close();
}

void Session::fireRead(SocketChannel* channel)
{
	// packet ??
	//BufferList& reader = channel->getReader();
	// 先解析出一个Packet
	// 放到队列里处理
	// 校验 parse
	//PacketEvent* ev = new PacketEvent();
}

void Session::fireWrite(SocketChannel* channel)
{
}

void Session::fireConnect(SocketChannel* channel)
{
}

void Session::fireError(SocketChannel* channel)
{
	Server::get().kick(this);
}

CUTE_NS_END

