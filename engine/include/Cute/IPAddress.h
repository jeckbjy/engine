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
    static IPAddress any(bool IPv6 = false) noexcept;
    static IPAddress local(bool IPv6 = false) noexcept;
    static IPAddress broadcast() noexcept;

	IPAddress(bool IPv6 = false) noexcept;
    IPAddress(const uint8 bytes[], bool IPv6 = false) noexcept;
    // ipv6
    IPAddress(const uint16 bytesp[8]) noexcept;
    // ipv4
    IPAddress(uint8 addr1, uint8 addr2, uint8 addr3, uint8 addr4) noexcept;
    // ipv6
    IPAddress (uint16 a1, uint16 a2, uint16 a3, uint16 a4, uint16 a5, uint16 a6, uint16 a7, uint16 a8) noexcept;
    // IPv4 address from a packed 32-bit integer
    IPAddress(uint32 asNativeEndian32Bit) noexcept;
    // ipv4:"1.2.3.4",ipv6:1:2:3:4:5:6:7:8
    IPAddress(const String& addr);
	~IPAddress();
    
    String toString() const;

//    bool isWildcard() const;
//    bool isBroadcast() const;
//    bool isLoopback() const;
//    bool isMulticast() const;
//    bool isUnicast() const;
//    bool isLinkLocal() const;
//    bool isSiteLocal() const;
//    bool isIPv4Compatible() const;
//    bool isIPv4Mapped() const;
//    bool isWellKnownMC() const;
//    bool isNodeLocalMC() const;
//    bool isLinkLocalMC() const;
//    bool isSiteLocalMC() const;
//    bool isOrgLocalMC() const;
//    bool isGlobalMC() const;
    
	bool operator == (const IPAddress& addr) const noexcept;
	bool operator != (const IPAddress& addr) const noexcept;
//	bool operator <  (const IPAddress& addr) const;
//	bool operator <= (const IPAddress& addr) const;
//	bool operator >  (const IPAddress& addr) const;
//	bool operator >= (const IPAddress& addr) const;

//	IPAddress operator & (const IPAddress& addr) const;
//	IPAddress operator | (const IPAddress& addr) const;
//	IPAddress operator ^ (const IPAddress& addr) const;
//	IPAddress operator ~ () const;

private:
    /** Method used to zero the remaining bytes of the address array when creating IPv4 addresses */
    void zeroUnusedBytes()
    {
        for (int i = 4; i < 16; ++i)
            m_address[i] = 0;
    }
    
private:
    union ByteUnion
    {
        uint16 combined;
        uint8  split[2];
    };
    
    uint8 m_address[16];
    bool  m_isIPv6;
};

//
// inlines
//
inline IPAddress IPAddress::any(bool IPv6) noexcept
{
    return IPAddress (IPv6);
}

inline IPAddress IPAddress::broadcast() noexcept
{
    return IPAddress (255, 255, 255, 255);
}

inline IPAddress IPAddress::local(bool IPv6) noexcept
{
    return IPv6 ? IPAddress (0, 0, 0, 0, 0, 0, 0, 1) : IPAddress (127, 0, 0, 1);
}

CUTE_NS_END
