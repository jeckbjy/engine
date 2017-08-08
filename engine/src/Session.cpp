//! Server
#include "Cute/Session.h"
#include "Cute/SessionFilterChain.h"
#include "Cute/Error.h"

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

void Session::sendBytes(const BufferList &data)
{
    m_socket->send(data);
}

void Session::send(Object *data)
{
    m_filters->doSend(this, data);
}

void Session::connect(const SocketAddress& addr)
{
	m_peer = addr;
    m_filters->doConnect(this, addr);
}

void Session::reconnect()
{
    m_filters->doConnect(this, m_peer);
}

void Session::shutdown(int how /* = SHUT_RD */)
{
//    m_filters->doShutdown(this, how);
	m_socket->shutdown(how);
}

void Session::close()
{
    m_filters->doClose(this);
}

void Session::fireRead(SocketChannel* channel)
{
    m_filters->onRead(this);
}

void Session::fireWrite(SocketChannel* channel)
{
    m_filters->onSend(this);
}

void Session::fireConnect(SocketChannel* channel)
{
    m_filters->onConnect(this);
}

void Session::fireError(SocketChannel* channel)
{
    m_socket->close();
    m_filters->onError(this, Error::last());
}

void Session::doSend(const BufferList &data)
{
    m_socket->send(data);
}

void Session::doConnect(const SocketAddress &addr)
{
    m_socket->connect(addr);
}

void Session::doClose()
{
    m_socket->close();
}

bool Session::hasValue(const String &key) const
{
    return m_values.find(key) != m_values.end();
}

bool Session::getValue(const String &key, Variant &value) const
{
    VariantMap::const_iterator itor = m_values.find(key);
    if(itor != m_values.end())
    {
        value = itor->second;
        return true;
    }
    
    return false;
}

void Session::setValue(const String &key, const Variant &value)
{
    m_values[key] = value;
}

CUTE_NS_END
