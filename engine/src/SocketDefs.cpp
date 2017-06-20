//! Network/Core
#include "Cute/SocketDefs.h"
#include "Cute/Error.h"
#include "Cute/Exception.h"
#include "Cute/SocketAddress.h"

CUTE_NS_BEGIN

#if defined(_WIN32)
LPFN_CONNECTEX FUN_CONNECTEX = NULL;
LPFN_ACCEPTEX  FUN_ACCEPTEX = NULL;
struct Network
{
	Network()
	{
		// 注册
		WSADATA wsa_data;
		::WSAStartup(MAKEWORD(2, 2), &wsa_data);

		// 获得函数指针
		socket_t sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, 0, 0, WSA_FLAG_OVERLAPPED);
		assert(sock != INVALID_SOCKET);

		DWORD bytes = 0;
		GUID guid_connectex = WSAID_CONNECTEX;
		DWORD res = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guid_connectex, sizeof(guid_connectex),
			&FUN_CONNECTEX, sizeof(FUN_CONNECTEX), &bytes, 0, 0);
		assert(res != SOCKET_ERROR);
		if (res == SOCKET_ERROR)
			FUN_CONNECTEX = NULL;

		GUID guid_acceptex = WSAID_ACCEPTEX;
		res = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guid_acceptex, sizeof(guid_acceptex),
			&FUN_ACCEPTEX, sizeof(FUN_ACCEPTEX), &bytes, 0, 0);
		assert(res != SOCKET_ERROR);
		if (res == SOCKET_ERROR)
			FUN_ACCEPTEX = NULL;

		::closesocket(sock);
	}
	~Network()
	{
		::WSACleanup();
	}
};

static Network gNetwork;

#endif

socket_t sock_create(int af, int type, int proto)
{
	socket_t sock;
#ifdef _WIN32
	sock = ::WSASocket(af, type, proto, 0, 0, WSA_FLAG_OVERLAPPED);
#else
	sock = ::socket(af, type, proto);
#endif

	// 默认是异步的，同步的需要外边单独设置一次
	sock_setblocking(sock, false);
	return sock;
}

socket_t sock_accept(socket_t listener, sockaddr* addr, socklen_t* len)
{
	socket_t sock;
	do
	{
		sock = ::accept(listener, addr, len);
	} while (sock == INVALID_SOCKET && Error::isLast(ERR_INTERRUPTED));
	return sock;
}

void sock_bind(socket_t sock, const struct sockaddr* addr, int len)
{
	int rc = ::bind(sock, addr, len);
	if (rc != 0)
		throw NetException("socket bind error!");
}

void sock_listen(socket_t sock, int backlog)
{
	int rc = ::listen(sock, backlog);
	if (rc != 0)
		throw NetException("socket listen error!");
}

int sock_close(socket_t sock)
{
#ifdef _WIN32
	return ::closesocket(sock);
#else
	return ::close(sock);
#endif
}

int sock_recv(socket_t sock, void* buff, int size, int flags /* = 0 */)
{
	int rc;
	do
	{
		rc = ::recv(sock, (char*)buff, size, flags);
	} while (rc < 0 && Error::last() == ERR_INTERRUPTED);
	return rc;
}

int sock_send(socket_t sock, const void* buff, int size, int flags /* = 0 */)
{
	int rc;
	do
	{
		rc = ::send(sock, (const char*)buff, size, flags);
	} while (rc < 0 && Error::last() == ERR_INTERRUPTED);
	return rc;
}

int sock_recvfrom(socket_t sock, void* buff, int size, int flags, struct sockaddr* from, int fromlen)
{
	int rc;
	do
	{
		rc = ::recvfrom(sock, (char*)buff, size, flags, from, &fromlen);
	} while (rc < 0 && Error::last() == ERR_INTERRUPTED);
	return rc;
}

int sock_sendto(socket_t sock, const void* buff, int size, int flags, const struct sockaddr* to, int tolen)
{
	int rc;
	do
	{
		rc = ::sendto(sock, (const char*)(buff), size, flags, to, tolen);
	} while (rc < 0 && Error::last() == ERR_INTERRUPTED);
	return rc;
}

int sock_connect(socket_t sock, const struct sockaddr* addr, int len)
{
	int rc;
	do
	{
		rc = ::connect(sock, addr, len);
	} while (rc != 0 && Error::isLast(ERR_INTERRUPTED));
	return rc;
}

int sock_ioctl(socket_t sock, ioctl_req_t request, void* arg)
{
#ifdef _WIN32
	return ::ioctlsocket(sock, request, reinterpret_cast<u_long*>(&arg));
#else
	return ::ioctl(sock, request, arg);
#endif
}

int sock_readable(socket_t sock)
{
	int bytes = 0;
	sock_ioctl(sock, FIONREAD, &bytes);
	return bytes;
}

void sock_setoption(socket_t sock, int level, int option, const void* value, socklen_t length)
{
	int rc = ::setsockopt(sock, level, option, reinterpret_cast<const char*>(value), length);
	if (rc != 0)
		throw NetException("setsockopt fail!" , Error::getMessage());
}

void sock_getoption(socket_t sock, int level, int option, void* value, socklen_t length)
{
	int rc = ::getsockopt(sock, level, option, reinterpret_cast<char*>(value), &length);
	if (rc != 0)
		throw NetException("getsockopt fail!", Error::getMessage());
}

void sock_setoption(socket_t sock, int level, int option, int value)
{
	sock_setoption(sock, level, option, &value, sizeof(value));
}

void sock_getoption(socket_t sock, int level, int option, int& value)
{
	sock_getoption(sock, level, option, &value, sizeof(value));
}

int sock_getoption(socket_t sock, int level, int option)
{
	int value;
	sock_getoption(sock, level, option, &value, sizeof(value));
	return value;
}

void sock_setreuseport(socket_t sock, bool flag)
{
#ifdef SO_REUSEPORT
	sock_setoption(sock, SOL_SOCKET, SO_REUSEPORT, flag);
#endif
}

bool sock_getreuseport(socket_t sock)
{
#ifdef SO_REUSEPORT
	return sock_getoption(sock, SOL_SOCKET, SO_REUSEPORT) != 0;
#else
	return false;
#endif
}

void sock_setlinger(socket_t sock, bool on, int seconds)
{
	struct linger l;
	l.l_onoff = on ? 1 : 0;
	l.l_linger = (u_short)seconds;
	sock_setoption(sock, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
}

void sock_getlinger(socket_t sock, bool& on, int& seconds)
{
	struct linger l;
	sock_getoption(sock, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
	on = l.l_onoff != 0;
	seconds = l.l_linger;
}

void sock_setblocking(socket_t sock, bool flag)
{
	int arg = flag ? 0 : 1;
	sock_ioctl(sock, FIONBIO, &arg);
}

void sock_getlocal(socket_t sock, SocketAddress& addr)
{
	char buffer[sizeof(sockaddr_in6)];
	struct sockaddr* sa = (struct sockaddr*)buffer;
	socklen_t len = 0;
	int rc = ::getsockname(sock, sa, &len);
	if (rc == 0)
	{
		addr = SocketAddress(sa, len);
	}
}

void sock_getpeer(socket_t sock, SocketAddress& addr)
{
	char buffer[sizeof(sockaddr_in6)];
	struct sockaddr* sa = (struct sockaddr*)buffer;
	socklen_t len = 0;
	int rc = ::getpeername(sock, sa, &len);
	if (rc == 0)
	{
		addr = SocketAddress(sa, len);
	}
}

CUTE_NS_END
