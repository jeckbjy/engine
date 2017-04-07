//! Network/Sockets
#include "Cute/TCPSocketChannel.h"
#include "Cute/IORequest.h"
#include "Cute/IOLoop.h"

CUTE_NS_BEGIN

TCPSocketChannel::TCPSocketChannel(IOLoop* loop)
	: SocketChannel(loop)
	, m_reqRead(NULL)
	, m_reqWrite(NULL)
	, m_reqConnect(NULL)
{
	setFlag(MASK_SOCKET, true);
}

TCPSocketChannel::~TCPSocketChannel()
{
	delete m_reqRead;
	delete m_reqWrite;
	delete m_reqConnect;
}

void TCPSocketChannel::open(socket_t sock)
{
	setSocket(sock);
	setFlag(MASK_ACTIVE, true);
	// attach??
}

void TCPSocketChannel::connect(const SocketAddress& addr)
{
	if (!isOpen())
	{
		socket_t sock = sock_create(addr.family(), SOCK_STREAM, IPPROTO_IP);
		setSocket(sock);
		attach();
	}

	if (isConnecting() || isActive())
		return;

	if (m_reqConnect == NULL)
		m_reqConnect = new ConnectReq(this);

	setFlag(MASK_CONNECTOR, true);
	setFlag(MASK_CONNECTING, true);

#ifdef _WIN32
	if (FUN_CONNECTEX == NULL)
		throw NetException("CONNECTEX  is null");

	bind(SocketAddress(), true);
	DWORD result, error;
	result = FUN_CONNECTEX(socket(), addr.address(), addr.length(), 0, 0, 0, m_reqConnect->getData());
	error = ::WSAGetLastError();
	if (!result && error != WSA_IO_PENDING)
		m_loop->post(m_reqConnect, error);
#else
	modify(EVENT_OUT);
	if (sock_connect(socket(), addr.address(), addr.length()) != 0)
	{
		bool blocking = code == EINPROGRESS || code == EWOULDBLOCK;
		if (!blocking)
		{//fire error
			m_listener->fireError(this);
		}
	}
#endif
}

void TCPSocketChannel::send(const BufferList& data)
{
	Mutex::ScopedLock lock(m_mutex);
	m_writer.merge(data);
	doSend();
}

void TCPSocketChannel::doSend()
{
	bool result = m_writer.send(socket());
	if (result == false)
	{
		m_listener->fireError(this);
		return;
	}

	if (m_writer.empty())
	{
		m_listener->fireWrite(this);
	}
	else
	{
		if (m_reqWrite == NULL)
			m_reqWrite = new WriteReq(this);

#ifdef CUTE_OS_FAMILY_WINDOWS
		WSABUF buf = { 0, 0 };
		DWORD bytes;
		int   result = ::WSASend(socket(), &buf, 1, &bytes, 0, m_reqWrite->getData(), 0);
		DWORD error = ::WSAGetLastError();
		if (result != ERROR_SUCCESS && error != WSA_IO_PENDING)
		{
			m_loop->post(m_reqWrite, error, bytes);
		}
#else
		modify(EVENT_OUT);
#endif
	}
}

void TCPSocketChannel::recv()
{
	if (m_reqRead == NULL)
		m_reqRead = new ReadReq(this);
#ifdef CUTE_OS_FAMILY_WINDOWS
	DWORD bytes = 0, flag = 0;
	WSABUF	buf = { 0, 0 };
	int		result = ::WSARecv(socket(), &buf, 1, &bytes, &flag, m_reqRead->getData(), 0);
	DWORD	error = ::WSAGetLastError();
	if (result != ERROR_SUCCESS && error != WSA_IO_PENDING)
	{
		m_loop->post(m_reqRead, error, bytes);
	}
#else
	modify(EVENT_IN);
#endif
}

void TCPSocketChannel::shutdown(int how)
{
	::shutdown(socket(), how);
}

void TCPSocketChannel::setOption(const ChannelOption& option)
{
	socket_t sock = this->socket();
	if (option.isKindOf<SocketOption>())
	{
		const SocketOption* so = (const SocketOption*)&option;
		int value = so->getValue();
		switch (so->getType())
		{
		case MASK_SO_BLOCKING:
			sock_setblocking(sock, value != 0);
			break;
		case MASK_SO_NODELAY:
			sock_setoption(sock, IPPROTO_TCP, TCP_NODELAY, value);
			break;
		case MASK_SO_KEEPALIVE:
			sock_setoption(sock, SOL_SOCKET, TCP_KEEPALIVE, value);
			break;
		case MASK_SO_OOBINLINE:
			sock_setoption(sock, SOL_SOCKET, SO_OOBINLINE, value);
			break;
		case MASK_SO_BROADCAST:
			sock_setoption(sock, SOL_SOCKET, SO_BROADCAST, value);
			break;
		case MASK_SO_REUSE_ADDR:
			sock_setoption(sock, SOL_SOCKET, SO_REUSEADDR, value);
			break;
		case MASK_SO_REUSE_PORT:
			sock_setreuseport(sock, value != 0);
			break;
		case MASK_SO_SNDBUF:
			sock_setoption(sock, SOL_SOCKET, SO_SNDBUF, value);
			break;
		case MASK_SO_RCVBUF:
			sock_setoption(sock, SOL_SOCKET, SO_RCVBUF, value);
			break;
		default:
			throw InvalidArgumentException("setOption!!");
		}
	}
	else if (option.isKindOf<LingerOption>())
	{
		const LingerOption* so = (const LingerOption*)&option;
		sock_setoption(sock, SOL_SOCKET, SO_LINGER, so->getValue(), sizeof(linger));
	}
	else
	{
		throw InvalidArgumentException("setOption!!");
	}
}

void TCPSocketChannel::getOption(ChannelOption& option)
{
	socket_t sock = this->socket();

	if (option.isKindOf<SocketOption>())
	{
		SocketOption* so = (SocketOption*)&option;
		int value = 0;
		switch (so->getType())
		{
		case MASK_SO_NODELAY:
			sock_getoption(sock, IPPROTO_TCP, TCP_NODELAY, value);
			break;
		case MASK_SO_KEEPALIVE:
			sock_getoption(sock, SOL_SOCKET, TCP_KEEPALIVE, value);
			break;
		case MASK_SO_OOBINLINE:
			sock_getoption(sock, SOL_SOCKET, SO_OOBINLINE, value);
			break;
		case MASK_SO_BROADCAST:
			sock_getoption(sock, SOL_SOCKET, SO_BROADCAST, value);
			break;
		case MASK_SO_REUSE_ADDR:
			sock_getoption(sock, SOL_SOCKET, SO_REUSEADDR, value);
			break;
		case MASK_SO_REUSE_PORT:
			value = sock_getreuseport(sock) ? 1 : 0;
			break;
		case MASK_SO_SNDBUF:
			sock_getoption(sock, SOL_SOCKET, SO_SNDBUF, value);
			break;
		case MASK_SO_RCVBUF:
			sock_getoption(sock, SOL_SOCKET, SO_RCVBUF, value);
			break;
		default:
			throw InvalidArgumentException("setOption!!");
		}

		so->setValue(value);
	}
	else if (option.isKindOf<LingerOption>())
	{
		LingerOption* so = (LingerOption*)&option;
		sock_getoption(sock, SOL_SOCKET, SO_LINGER, so->getValue(), sizeof(linger));
	}
	else
	{
		throw InvalidArgumentException("setOption!!");
	}
}

SocketAddress TCPSocketChannel::localAddress() const
{
	SocketAddress addr;
	sock_getlocal(socket(), addr);
	return addr;
}

SocketAddress TCPSocketChannel::peerAddress() const
{
	SocketAddress addr;
	sock_getpeer(socket(), addr);
	return addr;
}

void TCPSocketChannel::selection(int events)
{
	if (events & EVENT_IN)
	{
		completed(m_reqRead);
	}

	if (events & EVENT_OUT)
	{
		if (isConnecting())
		{
			completed(m_reqConnect);
		}

		completed(m_reqWrite);
	}
}

void TCPSocketChannel::completed(IORequest* req)
{
	if (req == NULL)
		throw InvalidArgumentException("TCPSocketChannel null operation!!");

	if (!req->isSuccess())
	{// 发生错误
		if (isConnecting())
			setFlag(MASK_CONNECTING, false);

		setFlag(MASK_ACTIVE, false);
		m_listener->fireError(this);
		return;
	}

	if (req->isKindOf<ConnectReq>())
	{// 连接成功
		setFlag(MASK_CONNECTING, false);
		setFlag(MASK_ACTIVE, true);
		m_listener->fireConnect(this);
	}
	else if (req->isKindOf<WriteReq>())
	{// 可写
		doSend();
	}
	else if (req->isKindOf<ReadReq>())
	{// 可读
		// 为零时对方关闭连接??
		bool result = m_reader.recv(socket());
		if (result)
			m_listener->fireRead(this);
		else
			m_listener->fireError(this);
	}
}

LingerOption::LingerOption()
{
	m_linger.l_onoff = 0;
	m_linger.l_linger = 0;
}

LingerOption::LingerOption(bool on, int second)
{
	setValue(on, second);
}

void LingerOption::setValue(bool on, int second)
{
	m_linger.l_onoff = on ? 1 : 0;
	m_linger.l_linger = (u_short)second;
}

CUTE_NS_END
