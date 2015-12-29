#include "Socket.h"

CU_NS_BEGIN

socket_t Socket::create_socket(int af /* = AF_INET */, int type /* = SOCK_STREAM */, int proto /* = IPPROTO_IP */)
{
	socket_t sock;
#ifdef _WIN32
	sock = ::WSASocket(af, type, proto, 0, 0, WSA_FLAG_OVERLAPPED);
#else
	sock = ::socket(af, type, proto);
#endif
	if (sock == INVALID_SOCKET)
		throw std::runtime_error("create socket error");

	return sock;
}

void Socket::close_socket(socket_t sock)
{
	if (sock != INVALID_SOCKET)
	{
#ifdef _WIN32
		::closesocket(sock);
#else
		::close(sock);
#endif
	}
}

Socket::Socket(socket_t sock)
	:m_sock(sock)
{
}

Socket::Socket(int af, int type, int proto /* = IPPROTO_IP */)
{
	m_sock = create_socket(af, type, proto);
}

Socket::~Socket()
{
	close();
}

void Socket::create(int af /* = AF_INET */, int type /* = SOCK_STREAM */, int proto /* = IPPROTO_IP */, bool blocking)
{
	close();
	m_sock = create_socket(af, type, proto);
	setBlocking(blocking);
}

void Socket::set(socket_t sock)
{
	close();
	m_sock = sock;
}

void Socket::reset()
{
	m_sock = INVALID_SOCKET;
}

void Socket::close()
{
}

void Socket::shutdown(int how /* = SHUT_RDWR */)
{
	assert(m_sock != INVALID_SOCKET);
	int rc = ::shutdown(m_sock, how);
	if (rc != 0)
		throw std::runtime_error("shut down");
}

void Socket::bind(const SocketAddress& addr, bool reuse /* = false */)
{
	assert(m_sock != INVALID_SOCKET);
	if (reuse)
	{
		setReuseAddress(true);
		setReusePort(true);
	}
	int rc = ::bind(m_sock, addr.address(), addr.length());
	if (rc != 0)
		throw std::runtime_error("socket bind error.");
}

void Socket::listen(int backlog /* = 64 */)
{
	assert(m_sock != INVALID_SOCKET);
	int rc = ::listen(m_sock, backlog);
	if (rc != 0)
		throw std::runtime_error("socket listen error.");
}

socket_t Socket::accept(sockaddr* addr, socklen_t* len)
{
	socket_t sock;
	do 
	{
		sock = ::accept(m_sock, addr, len);
	} while (sock == INVALID_SOCKET && last_error() == ERR_INTERRUPTED);
	return sock;
}

int Socket::connect(const SocketAddress& addr)
{
	int rc;
	do 
	{
		rc = ::connect(m_sock, addr.address(), addr.length());
	} while (rc != 0 && last_error() == ERR_INTERRUPTED);
	return rc;
}

int Socket::send(const void* buf, int len, int flags /* = 0 */)
{
	assert(m_sock != INVALID_SOCKET);
	int rc;
	do
	{
		rc = ::send(m_sock, (const char*)buf, len, flags);
	} while (rc < 0 && last_error() == ERR_INTERRUPTED);
	return rc;
}

int Socket::recv(void* buf, int len, int flags /* = 0 */)
{
	assert(m_sock != INVALID_SOCKET);
	int rc;
	do
	{
		rc = ::recv(m_sock, (char*)buf, len, flags);
	} while (rc < 0 && last_error() == ERR_INTERRUPTED);
	return rc;
}

int Socket::sendTo(const SocketAddress& addr, const void* buf, int len, int flags /* = 0 */)
{
	assert(m_sock != INVALID_SOCKET);
	int rc;
	do
	{
		rc = ::sendto(m_sock, (const char*)(buf), len, flags, addr.address(), addr.length());
	} while (rc < 0 && last_error() == ERR_INTERRUPTED);
	return rc;
}

int Socket::recvFrom(SocketAddress& addr, void* buf, int len, int flags /* = 0 */)
{
	assert(m_sock != INVALID_SOCKET);
	socklen_t salen;
	int rc;
	do
	{
		rc = ::recvfrom(m_sock, (char*)buf, len, flags, addr.address(), &salen);
	} while (rc < 0 && last_error() == ERR_INTERRUPTED);
	return rc;
}

int Socket::available()
{
	// why cannot get bytes
	int result = 0;
	ioctl(FIONREAD, &result);
	return result;
}

SocketAddress Socket::address() const
{
	assert(m_sock != INVALID_SOCKET);
	char buffer[SocketAddress::MAX_ADDR_LEN];
	struct sockaddr* sa = (struct sockaddr*)buffer;
	socklen_t len = sizeof(buffer);
	int rc = ::getsockname(m_sock, sa, &len);
	if (rc == 0)
		return SocketAddress(sa, len);
	return SocketAddress();
}

SocketAddress Socket::peerAddress() const
{
	assert(m_sock != INVALID_SOCKET);
	char buffer[SocketAddress::MAX_ADDR_LEN];
	struct sockaddr* sa = (struct sockaddr*)buffer;
	socklen_t len = sizeof(buffer);
	int rc = ::getpeername(m_sock, sa, &len);
	if (rc == 0)
		return SocketAddress(sa, len);
	return SocketAddress();
}

void Socket::ioctl(ioctl_req_t request, void* arg)
{
#ifdef CU_OS_WIN
	int rc = ioctlsocket(m_sock, request, reinterpret_cast<u_long*>(&arg));
#else
	int rc = ::ioctl(m_sock, request, arg);
#endif
	if (rc != 0)
		throw std::runtime_error("ioctl error");
}

void Socket::setOption(int level, int option, const void* value, socklen_t lengh)
{
	assert(m_sock != INVALID_SOCKET);
	int rc = ::setsockopt(m_sock, level, option, reinterpret_cast<const char*>(value), lengh);
	if (rc != 0)
		throw std::runtime_error("ioctl");
}

void Socket::getOption(int level, int option, void* value, socklen_t length) const
{
	assert(m_sock != INVALID_SOCKET);
	int rc = ::getsockopt(m_sock, level, option, reinterpret_cast<char*>(value), &length);
	if (rc == -1)
		throw std::runtime_error("getsockopt error");
}

void Socket::setOptionFlag(int level, int option, bool flag)
{
	int value = flag ? 1 : 0;
	setOption(level, option, &value, sizeof(value));
}

bool Socket::getOptionFlag(int level, int option) const
{
	int value;
	getOption(level, option, &value, sizeof(value));
	return value != 0;
}

void Socket::setOptionInteger(int level, int option, int value)
{
	setOption(level, option, &value, sizeof(value));
}

int  Socket::getOptionInteger(int level, int option) const
{
	int result;
	getOption(level, option, &result, sizeof(int));
	return result;
}

void Socket::setLinger(bool on, int seconds)
{
	struct linger l;
	l.l_onoff = on ? 1 : 0;
	l.l_linger = (u_short)seconds;
	setOption(SOL_SOCKET, SO_LINGER, &l, sizeof(l));
}

void Socket::getLinger(bool& on, int& seconds) const
{
	struct linger l;
	getOption(SOL_SOCKET, SO_LINGER, &l, sizeof(l));
	on = l.l_onoff != 0;
	seconds = l.l_linger;
}

void Socket::setBlocking(bool flag)
{
	int arg = flag ? 0 : 1;
	ioctl(FIONBIO, &arg);
}

CU_NS_END