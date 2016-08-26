#pragma once
#include "API.h"
#include "SocketAddress.h"

CU_NS_BEGIN

class CU_API Socket
{
public:
	static socket_t create_socket(int af = AF_INET, int type = SOCK_STREAM, int proto = IPPROTO_IP);
	static void		close_socket(socket_t& sock);
	
	Socket(socket_t sock = INVALID_SOCKET);
	Socket(int af, int type, int proto = IPPROTO_IP);
	~Socket();

	bool invalid() const { return m_sock == INVALID_SOCKET; }

	void create(int af = AF_INET, int type = SOCK_STREAM, int proto = IPPROTO_IP, bool blocking = false);
	void set(socket_t sock);
	void reset();
	void close();
	void shutdown(int how = SHUT_RDWR);

	void bind(const SocketAddress& addr, bool reuse = false);
	void listen(int backlog = 64);

	socket_t accept(sockaddr* addr = NULL, socklen_t* len = NULL);
	int connect(const SocketAddress& addr);
	int send(const void* buf, int len, int flags = 0);
	int recv(void* buf, int len, int flags = 0);
	int sendTo(const SocketAddress& addr, const void* buf, int len, int flags = 0);
	int recvFrom(SocketAddress& addr, void* buf, int len, int flags = 0);
	int available();

	SocketAddress address() const;
	SocketAddress peerAddress() const;

	void ioctl(ioctl_req_t request, void* arg);

	void setOption(int level, int option, const void* value, socklen_t lengh);
	void getOption(int level, int option, void* value, socklen_t length) const;

	// 辅助函数，内部调用setOption
	void setOptionFlag(int level, int option, bool flag);
	bool getOptionFlag(int level, int option) const;
	void setOptionInteger(int level, int option, int value);
	int  getOptionInteger(int level, int option) const;

	void setLinger(bool on, int seconds);
	void getLinger(bool& on, int& seconds) const;

	void setNoDelay(bool flag);
	bool getNoDelay() const;

	void setKeepAlive(bool flag);
	bool getKeepAlive() const;

	void setReuseAddress(bool flag);
	bool getReuseAddress() const;

	void setReusePort(bool flag);
	bool getReusePort() const;

	void setOOBInline(bool flag);
	bool getOOBInline() const;

	void setBroadcast(bool flag);
	bool getBroadcast() const;

	void setBlocking(bool flag);

	void setSendBufferSize(int size);
	int  getSendBufferSize() const;
	void setReceiveBufferSize(int size);
	int  getReceiveBufferSize() const;

	int getError() const;
	socket_t native() const { return m_sock; }

	operator socket_t() const { return m_sock; }
	bool operator!() const { return m_sock != INVALID_SOCKET; }
	Socket& operator=(socket_t sock) { set(sock); return *this; }

private:
	socket_t m_sock;
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
inline void Socket::setNoDelay(bool flag)
{
	setOptionFlag(IPPROTO_TCP, TCP_NODELAY, flag);
}

inline bool Socket::getNoDelay() const
{
	return getOptionFlag(IPPROTO_TCP, TCP_NODELAY);
}

inline void Socket::setKeepAlive(bool flag)
{
	setOptionFlag(SOL_SOCKET, SO_KEEPALIVE, flag);
}

inline bool Socket::getKeepAlive() const
{
	return getOptionFlag(SOL_SOCKET, SO_KEEPALIVE);
}

inline void Socket::setReuseAddress(bool flag)
{
	setOptionFlag(SOL_SOCKET, SO_REUSEADDR, flag);
}

inline bool Socket::getReuseAddress() const
{
	return getOptionFlag(SOL_SOCKET, SO_REUSEADDR);
}

inline void Socket::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
	setOptionFlag(SOL_SOCKET, SO_REUSEPORT, flag);
#endif
}

inline bool Socket::getReusePort() const
{
#ifdef SO_REUSEPORT
	return getOptionFlag(SOL_SOCKET, SO_REUSEPORT);
#else
	return false;
#endif
}

inline void Socket::setOOBInline(bool flag)
{
	setOptionFlag(SOL_SOCKET, SO_OOBINLINE, flag);
}

inline bool Socket::getOOBInline() const
{
	return getOptionFlag(SOL_SOCKET, SO_OOBINLINE);
}

inline void Socket::setBroadcast(bool flag)
{
	setOptionFlag(SOL_SOCKET, SO_BROADCAST, flag);
}

inline bool Socket::getBroadcast() const
{
	return getOptionFlag(SOL_SOCKET, SO_BROADCAST);
}

inline void Socket::setSendBufferSize(int size)
{
	setOptionInteger(SOL_SOCKET, SO_SNDBUF, size);
}

inline int Socket::getSendBufferSize() const
{
	return getOptionInteger(SOL_SOCKET, SO_SNDBUF);
}

inline void Socket::setReceiveBufferSize(int size)
{
	setOptionInteger(SOL_SOCKET, SO_RCVBUF, size);
}

inline int Socket::getReceiveBufferSize() const
{
	return getOptionInteger(SOL_SOCKET, SO_RCVBUF);
}

inline int Socket::getError() const
{
	return getOptionInteger(SOL_SOCKET, SO_ERROR);
}

CU_NS_END
