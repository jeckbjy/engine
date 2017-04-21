//! Server
#include "Cute/Session.h"
#include "Cute/Server.h"

CUTE_NS_BEGIN

Session::Session(Protocal* protocal, SocketChannel* socket, uint32 id, uint32 type)
	: m_socket(socket)
	, m_id(id)
	, m_type(type)
	, m_data(NULL)
	, m_protocal(protocal)
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

void Session::addPending(int pending)
{
	Server::get().addPending(this, pending);
}

void Session::fireRead(SocketChannel* channel)
{
	// 先解析消息
	BufferList& buffer = channel->getReader();
	buffer.seek(0, SEEK_SET);

	m_protocal->process(this, buffer);
}

void Session::fireWrite(SocketChannel* channel)
{
	addPending(Server::PE_SEND);
}

void Session::fireConnect(SocketChannel* channel)
{
	addPending(Server::PE_CONNECT);
}

void Session::fireError(SocketChannel* channel)
{
	m_socket->close();
	addPending(Server::PE_ERROR);
}

CUTE_NS_END
