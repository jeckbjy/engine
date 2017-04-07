//! Network/Core
#include "Cute/SocketAddress.h"

CUTE_NS_BEGIN

#ifdef _WIN32
void inet_aton(const char* str, struct in_addr* addr4)
{
	addr4->s_addr = inet_addr(str);
}
#endif

SocketAddress::SocketAddress()
{
	::memset(&m_addr6, 0, sizeof(m_addr6));
	m_addr4.sin_addr.s_addr = htonl(INADDR_ANY);
	m_addr4.sin_family = AF_INET;
	m_addr4.sin_port = htons(0);
}

SocketAddress::SocketAddress(uint16_t hostPort)
{
	::memset(&m_addr6, 0, sizeof(m_addr6));
	m_addr4.sin_addr.s_addr = htonl(INADDR_ANY);
	m_addr4.sin_family = AF_INET;
	m_addr4.sin_port = htons(hostPort);
}

SocketAddress::SocketAddress(const String& host)
{
	parse(host);
}

SocketAddress::SocketAddress(const String& hostAddr, uint16_t hostPort)
{
	parse(hostAddr);
	m_addr4.sin_port = htons(hostPort);
}

SocketAddress::SocketAddress(const struct sockaddr* addr, socklen_t len)
{
	assert(len <= MAX_ADDR_LEN);
	memcpy(&m_addr4, addr, len);
}

SocketAddress::~SocketAddress()
{

}

void SocketAddress::swap(SocketAddress& other)
{
	std::swap(m_addr6, other.m_addr6);
}

void SocketAddress::parse(const String& addr)
{
	if (addr.empty())
		return;
	// split host and port
	String host;
	size_t index = addr.rfind(':');
	if (index != addr.npos){
		host = addr.substr(0, index);
		// port
		const char* str_port = &addr[index + 1];
		//
		unsigned port = 0;
		char tmp;
		bool res = sscanf(str_port, "%u%c", &port, &tmp) == 1;
		if (res && port <= 0xFFFF){
			port = htons((uint16_t)port);
		}		else{
			struct servent* se = getservbyname(str_port, NULL);
			if (se)
				port = se->s_port;
		}
		m_addr4.sin_port = port;
	}
	else{
		host = addr;
	}
	// check host
	int af;
	if (host[0] == '['){
		index = host.rfind(']');
		if (index == String::npos)
			throw std::runtime_error("parse ipv6 host error.");
		host = host.substr(1, index);
		af = AddressFamily::IPv6;
	}
	else{
		af = AddressFamily::IPv4;
	}

	// do parse
	m_addr4.sin_family = af;
	if (af == AddressFamily::IPv4){
		inet_aton(host.c_str(), &m_addr4.sin_addr);
	}
	else{
#ifdef _WIN32
		struct addrinfo* info;
		struct addrinfo hints;
		std::memset(&hints, 0, sizeof(hints));
		hints.ai_flags = AI_NUMERICHOST;
		int rc = getaddrinfo(host.c_str(), NULL, &hints, &info);
		if (rc == 0)
		{
			struct sockaddr_in6* addr_in6 = reinterpret_cast<struct sockaddr_in6*>(info->ai_addr);
			memcpy(&m_addr6, addr_in6, sizeof(m_addr6));
			freeaddrinfo(info);
		}
#else
		String::size_type pos = host.find('%');
		if (String::npos == pos){
			inet_pton(AF_INET6, addr.data(), &m_addr6.sin6_addr);
		}
		else{
			String::size_type start = ('[' == host[0]) ? 1 : 0;
			String unscopedAddr(host, start, pos - start);
			String scope(host, pos + 1, host.size() - start - pos);
			if (!(m_addr6.sin6_scope_id = if_nametoindex(scope.c_str())))
				return;
			inet_pton(AF_INET6, unscopedAddr.c_str(), &m_addr6.sin6_addr);
		}
#endif
	}
}

String SocketAddress::toString() const
{
	String result;
	char buff[20];
	buff[19] = 0;
	inet_ntop(family(), (void*)&m_addr6, buff, 20);

	if (family() == AddressFamily::IPv6)
		result.append("[");

	result.append(buff);

	if (family() == AddressFamily::IPv6)
		result.append("]");
	result.append(":");
	char buffer[10];
	sprintf(buffer, "%u", port());
	result.append(buffer);
	return result;
}

CUTE_NS_END