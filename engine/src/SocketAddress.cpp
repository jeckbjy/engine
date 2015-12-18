#include "SocketAddress.h"

CU_NS_BEGIN

#ifdef CU_OS_WIN
void inet_aton(const char* str, struct in_addr* addr4)
{
	addr4->s_addr = inet_addr(str);
}
#endif

SocketAddress::SocketAddress(ushort port /* = 0 */)
{
	::memset(&m_addr6, 0, sizeof(m_addr6));
	m_addr4.sin_addr.s_addr = htonl(INADDR_ANY);
	m_addr4.sin_family = AF_INET;
	m_addr4.sin_port = htons(port);
}

SocketAddress::SocketAddress(const StringPiece& host_port)
{
	parse(host_port);
}

SocketAddress::SocketAddress(const StringPiece& addr, ushort port)
{
	parse(addr);
	m_addr4.sin_port = htons(port);
}

SocketAddress::SocketAddress(sockaddr* addr, socklen_t len)
{
	assert(len <= MAX_ADDR_LEN);
	memcpy(&m_addr4, addr, len);
}

void SocketAddress::swap(SocketAddress& addr)
{
	std::swap(m_addr6, addr.m_addr6);
}

void SocketAddress::parse(const StringPiece& addr)
{
	if (addr.empty())
		return;
	StringPiece host_tmp;
	size_t index = addr.rfind(':');
	if (index != addr.npos){
		host_tmp = addr.slice(0, index);
		// port
		const char* str_port = addr.slice(index + 1).data();
		//
		unsigned port;
		char tmp;
		bool res = sscanf(str_port, "%u%c", &port, &tmp) == 1;
		if (res && port <= 0xFFFF){
			port = htons((ushort)port);
		}else{
			struct servent* se = getservbyname(str_port, NULL);
			if (se)
				port = se->s_port;
		}
	}else{
		host_tmp = addr;
	}
	// check host
	int af;
	if (host_tmp[0] == '['){
		if (host_tmp.back() != ']')
			throw std::runtime_error("parse ipv6 host error.");
		host_tmp = host_tmp.slice(1, host_tmp.size() - 2);
		af = IPv6;
	}else{
		af = IPv4;
	}
	m_addr4.sin_family = af;
	String host = host_tmp.toString();
	if (af == IPv4){
		inet_aton(host.c_str(), &m_addr4.sin_addr);
	}else{
#ifdef CU_OS_WIN
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

	if (family() == IPv6)
		result.append("[");

	result.append(buff);

	if (family() == IPv6)
		result.append("]");
	result.append(":");
	char buffer[10];
	sprintf(buffer, "%u", port());
	result.append(buffer);
	return result;
}

CU_NS_END