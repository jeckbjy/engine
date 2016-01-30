#pragma once
#include "API.h"

CU_NS_BEGIN

class CU_API SocketAddress
{
public:
	enum Family
	{
		IPv4 = AF_INET,
		IPv6 = AF_INET6
	};

	enum { MAX_ADDR_LEN = sizeof(sockaddr_in6) };

	SocketAddress(ushort port = 0);
	SocketAddress(const String& host_port);
	SocketAddress(const String& addr, ushort port);
	SocketAddress(sockaddr* addr, socklen_t len);

	String toString() const;

	void swap(SocketAddress& addr);
	void parse(const String& addr);

	const in_addr&  addr4() const { return m_addr4.sin_addr; }
	const in6_addr& addr6() const { return m_addr6.sin6_addr; }
	const sockaddr* address() const { return (const sockaddr*)&m_addr4; }
	sockaddr* address() { return (sockaddr*)&m_addr4; }
	socklen_t length() const { return family() == IPv4 ? sizeof(m_addr4) : sizeof(m_addr6); }
	int family() const { return m_addr4.sin_family; }
	ushort port() const { return m_addr4.sin_port; }

private:
	union
	{
		sockaddr_in	 m_addr4;
		sockaddr_in6 m_addr6;
	};
};

CU_NS_END