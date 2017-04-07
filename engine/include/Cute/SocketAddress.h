#pragma once
#include "Cute/SocketDefs.h"

CUTE_NS_BEGIN

/// This class represents an internet (IP) endpoint/socket
/// address. The address can belong either to the
/// IPv4 or the IPv6 address family and consists of a
/// host address and a port number.

/// Creates a SocketAddress from an IP address or host name and the
/// port number/service name. Host name/address and port number must
/// be separated by a colon. In case of an IPv6 address,
/// the address part must be enclosed in brackets.
///
/// Examples:
///     192.168.1.10:80
///     [::ffff:192.168.1.120]:2040
///     www.appinf.com:8080
///
/// On POSIX platforms supporting UNIX_LOCAL sockets, hostAndPort
/// can also be the absolute path of a local socket, starting with a 
/// slash, e.g. "/tmp/local.socket".
class CUTE_CORE_API SocketAddress
{
public:
	enum { MAX_ADDR_LEN = sizeof(sockaddr_in6) };

	SocketAddress();
	SocketAddress(uint16_t port);
	SocketAddress(const String& host);
	SocketAddress(const String& addr, uint16_t port);
	SocketAddress(const struct sockaddr* addr, socklen_t leng);
	~SocketAddress();

	void swap(SocketAddress& other);

	const struct sockaddr* address() const;
	sockaddr* address();
	socklen_t length() const;

	uint16_t port() const;
	int family() const;

	String toString() const;

private:
	void parse(const String& host);

private:
	union
	{
		char		 m_storage[MAX_ADDR_LEN];
		sockaddr_in	 m_addr4;
		sockaddr_in6 m_addr6;
		sockaddr_un	 m_addrun;
	};
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
inline const struct sockaddr* SocketAddress::address() const
{
	return (sockaddr*)&m_addr4;
}

inline sockaddr* SocketAddress::address()
{
	return (sockaddr*)&m_addr4;
}

inline socklen_t SocketAddress::length() const
{
	return family() == AF_INET ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
}

inline int SocketAddress::family() const
{
	return m_addr4.sin_family;
}

inline uint16_t SocketAddress::port() const
{
	return m_addr4.sin_port;
}

CUTE_NS_END
