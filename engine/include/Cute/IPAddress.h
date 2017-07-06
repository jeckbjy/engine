#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// This class represents an internet (IP) host
/// address. The address can belong either to the
/// IPv4 or the IPv6 address family.
///
/// Relational operators (==, !=, <, <=, >, >=) are
/// supported. However, you must not interpret any
/// special meaning into the result of these 
/// operations, other than that the results are
/// consistent.
///
/// Especially, an IPv4 address is never equal to
/// an IPv6 address, even if the IPv6 address is
/// IPv4 compatible and the addresses are the same.
///
/// IPv6 addresses are supported only if the target platform
/// supports IPv6.
class CUTE_CORE_API IPAddress
{
public:
	enum
	{
		MAX_ADDRESS_LENGTH = sizeof(in6_addr)
	};
    
    /** Populates a list of all the IP addresses that this machine is using. */
    static void findAllAddresses(Vector<IPAddress>& results, bool includeIPv6 = false);

	IPAddress();
	IPAddress(const String& addr);
	~IPAddress();

	bool isWildcard() const;
	bool isBroadcast() const;
	bool isLoopback() const;
	bool isMulticast() const;
	bool isUnicast() const;
	bool isLinkLocal() const;
	bool isSiteLocal() const;
	bool isIPv4Compatible() const;

	bool operator == (const IPAddress& addr) const;
	bool operator != (const IPAddress& addr) const;
	bool operator <  (const IPAddress& addr) const;
	bool operator <= (const IPAddress& addr) const;
	bool operator >  (const IPAddress& addr) const;
	bool operator >= (const IPAddress& addr) const;

	IPAddress operator & (const IPAddress& addr) const;
	IPAddress operator | (const IPAddress& addr) const;
	IPAddress operator ^ (const IPAddress& addr) const;
	IPAddress operator ~ () const;

private:
    union ByteUnion
    {
        uint16 combined;
        uint8  split[2];
    };
    
    uint8 m_address[16];
    bool  m_isIPv6;
};

CUTE_NS_END
