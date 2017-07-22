//! Network/Core
#include "Cute/IPAddress.h"
#include "Cute/Number.h"
#include "Cute/SharedLibrary.h"
#include "Cute/HeapBlock.h"

#if defined(CUTE_OS_FAMILY_WINDOWS)
#include <iptypes.h>
#elif defined(CUTE_OS_FAMILY_POSIX)
#include <net/if.h>
#endif

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOWS)
void IPAddress::findAllAddresses(Vector<IPAddress> &results, bool includeIPv6)
{
    SharedLibrary dll("iphlpapi.dll");
    CUTE_LOAD_FUNCTION(dll, GetAdaptersAddresses, getAdaptersAddresses, DWORD, (ULONG, ULONG, PVOID, PIP_ADAPTER_ADDRESSES, PULONG));
    if(getAdaptersAddresses == NULL)
        return;
    
    HeapBlock<IP_ADAPTER_ADDRESSES> addrs(1);
	ULONG len = sizeof(IP_ADAPTER_ADDRESSES);
    if(getAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, addrs, &len) == ERROR_BUFFER_OVERFLOW)
    {
        addrs.malloc(len, 1);
        if(getAdaptersAddresses (AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, addrs, &len) == NO_ERROR)
            return;
    }
    
    //
    for(PIP_ADAPTER_ADDRESSES adapter = addrs; adapter != NULL; adapter = adapter->Next)
    {
        /// UNICAST_ADDRESS
        PIP_ADAPTER_UNICAST_ADDRESS pUnicast = nullptr;
        for (pUnicast = adapter->FirstUnicastAddress; pUnicast != nullptr; pUnicast = pUnicast->Next)
        {
            if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
            {
                const sockaddr_in* sa_in = (sockaddr_in*)pUnicast->Address.lpSockaddr;
                IPAddress ip ((uint8*)&sa_in->sin_addr.s_addr, false);
				results.push_back(ip);
                //result.addIfNotAlreadyThere (ip);
            }
            else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6 && includeIPv6)
            {
                const sockaddr_in6* sa_in6 = (sockaddr_in6*)pUnicast->Address.lpSockaddr;
                
                ByteUnion temp;
                uint16 arr[8];
                
                for (int i = 0; i < 8; ++i)
                {
                    temp.split[0] = sa_in6->sin6_addr.u.Byte[i * 2 + 1];
                    temp.split[1] = sa_in6->sin6_addr.u.Byte[i * 2];
                    
                    arr[i] = temp.combined;
                }
                
                IPAddress ip (arr);
				results.push_back(ip);
                //result.addIfNotAlreadyThere (ip);
            }
        }
        
        /// ANYCAST_ADDRESS
        PIP_ADAPTER_ANYCAST_ADDRESS   pAnycast = nullptr;
        for (pAnycast = adapter->FirstAnycastAddress; pAnycast != nullptr; pAnycast = pAnycast->Next)
        {
            if (pAnycast->Address.lpSockaddr->sa_family == AF_INET)
            {
                const sockaddr_in* sa_in = (sockaddr_in*)pAnycast->Address.lpSockaddr;
                IPAddress ip ((uint8*)&sa_in->sin_addr.s_addr, false);
				results.push_back(ip);
                //result.addIfNotAlreadyThere (ip);
            }
            else if (pAnycast->Address.lpSockaddr->sa_family == AF_INET6 && includeIPv6)
            {
                const sockaddr_in6* sa_in6 = (sockaddr_in6*)pAnycast->Address.lpSockaddr;
                
                ByteUnion temp;
                uint16 arr[8];
                
                for (int i = 0; i < 8; ++i)
                {
                    temp.split[0] = sa_in6->sin6_addr.u.Byte[i * 2 + 1];
                    temp.split[1] = sa_in6->sin6_addr.u.Byte[i * 2];
                    
                    arr[i] = temp.combined;
                }
                
                IPAddress ip (arr);
				results.push_back(ip);
                //result.addIfNotAlreadyThere (ip);
            }
        }
        
        /// MULTICAST_ADDRESS
        PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = nullptr;
        for (pMulticast = adapter->FirstMulticastAddress; pMulticast != nullptr; pMulticast = pMulticast->Next)
        {
            if (pMulticast->Address.lpSockaddr->sa_family == AF_INET)
            {
                const sockaddr_in* sa_in = (sockaddr_in*)pMulticast->Address.lpSockaddr;
                IPAddress ip ((uint8*)&sa_in->sin_addr.s_addr, false);
				results.push_back(ip);
                //result.addIfNotAlreadyThere (ip);
            }
            else if (pMulticast->Address.lpSockaddr->sa_family == AF_INET6 && includeIPv6)
            {
                const sockaddr_in6* sa_in6 = (sockaddr_in6*)pMulticast->Address.lpSockaddr;
                
                ByteUnion temp;
                uint16 arr[8];
                
                for (int i = 0; i < 8; ++i)
                {
                    temp.split[0] = sa_in6->sin6_addr.u.Byte[i * 2 + 1];
                    temp.split[1] = sa_in6->sin6_addr.u.Byte[i * 2];
                    
                    arr[i] = temp.combined;
                }
                
                IPAddress ip (arr);
				results.push_back(ip);
                //result.addIfNotAlreadyThere (ip);
            }
        }
        /// end
    }
}
#elif defined(CUTE_OS_FAMILY_POSIX)
static void addAddress (const sockaddr_in* addr_in, Vector<IPAddress>& result)
{
    in_addr_t addr = addr_in->sin_addr.s_addr;
    
    if (addr != INADDR_NONE)
        result.push_back( IPAddress( ntohl(addr) ) );
//        result.addIfNotAlreadyThere (IPAddress (ntohl (addr)));
}

static void findIPAddresses (int sock, Vector<IPAddress>& results)
{
    ifconf cfg;
    HeapBlock<char> buffer;
    int bufferSize = 1024;
    
    do
    {
        bufferSize *= 2;
        buffer.calloc ((size_t) bufferSize);
        
        cfg.ifc_len = bufferSize;
        cfg.ifc_buf = buffer;
        
        if (ioctl (sock, SIOCGIFCONF, &cfg) < 0 && errno != EINVAL)
            return;
        
    } while (bufferSize < cfg.ifc_len + 2 * (int) (IFNAMSIZ + sizeof (struct sockaddr_in6)));
    
#if defined(CUTE_OS_MAC) || defined(CUTE_OS_IOS)
    while (cfg.ifc_len >= (int) (IFNAMSIZ + sizeof (struct sockaddr_in)))
    {
        if (cfg.ifc_req->ifr_addr.sa_family == AF_INET) // Skip non-internet addresses
            addAddress ((const sockaddr_in*) &cfg.ifc_req->ifr_addr, results);
        
        cfg.ifc_len -= IFNAMSIZ + cfg.ifc_req->ifr_addr.sa_len;
        cfg.ifc_buf += IFNAMSIZ + cfg.ifc_req->ifr_addr.sa_len;
    }
#else
    for (size_t i = 0; i < (size_t) cfg.ifc_len / (size_t) sizeof (struct ifreq); ++i)
    {
        const ifreq& item = cfg.ifc_req[i];
        
        if (item.ifr_addr.sa_family == AF_INET)
            addAddress ((const sockaddr_in*) &item.ifr_addr, results);
    }
#endif
}

void IPAddress::findAllAddresses(Vector<IPAddress> &results, bool includeIPv6)
{
    const int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock >= 0)
    {
        findIPAddresses(sock, results);
        ::close(sock);
    }
}
#endif

IPAddress::IPAddress(bool IPv6) noexcept
    : m_isIPv6(IPv6)
{
    memset(m_address, 0, sizeof(m_address));
}

IPAddress::IPAddress(const uint8 bytes[], bool IPv6) noexcept
: m_isIPv6(IPv6)
{
    for(int i = 0; i < (IPv6 ? 16 : 4); ++i)
        m_address[i] = bytes[i];
    
    if(!IPv6)
        zeroUnusedBytes();
}

IPAddress::IPAddress(const uint16 bytes[8]) noexcept
: m_isIPv6(true)
{
    ByteUnion temp;
    
    for (int i = 0; i < 8; ++i)
    {
        temp.combined = bytes[i];
        
        m_address[i * 2]     = temp.split[0];
        m_address[i * 2 + 1] = temp.split[1];
    }
}

IPAddress::IPAddress (uint8 a0, uint8 a1, uint8 a2, uint8 a3) noexcept : m_isIPv6 (false)
{
    m_address[0] = a0;  m_address[1] = a1;
    m_address[2] = a2;  m_address[3] = a3;
    
    zeroUnusedBytes();
}

IPAddress::IPAddress (uint16 a1, uint16 a2, uint16 a3, uint16 a4,
                      uint16 a5, uint16 a6, uint16 a7, uint16 a8) noexcept
: m_isIPv6 (true)

{
    uint16 array[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };
    
    ByteUnion temp;
    
    for (int i = 0; i < 8; ++i)
    {
        temp.combined = array[i];
        m_address[i * 2]     = temp.split[0];
        m_address[i * 2 + 1] = temp.split[1];
    }
}

IPAddress::IPAddress (uint32 n) noexcept
: m_isIPv6 (false)
{
    m_address[0] = (n >> 24);
    m_address[1] = (n >> 16) & 255;
    m_address[2] = (n >> 8) & 255;
    m_address[3] = (n & 255);
    
    zeroUnusedBytes();
}

IPAddress::IPAddress (const String& adr)
{
    m_isIPv6 = adr.contains (":");
    
    if (! m_isIPv6)
    {
//        StringArray tokens;
//        tokens.addTokens (adr, ".", String());
//        
//        for (int i = 0; i < 4; ++i)
//            m_address[i] = (uint8) tokens[i].getIntValue();
    }
    else
    {
//        StringArray tokens;
//        tokens.addTokens (adr.removeCharacters ("[]"), ":", String());
//        
//        if (tokens.contains (StringRef())) // if :: shorthand has been used
//        {
//            int idx = tokens.indexOf (StringRef());
//            tokens.set (idx, "0");
//            
//            while (tokens.size() < 8)
//                tokens.insert (idx, "0");
//        }
//        
//        for (int i = 0; i < 8; ++i)
//        {
//            ByteUnion temp;
//            temp.combined = (uint16) CharacterFunctions::HexParser<int>::parse (tokens[i].getCharPointer());
//            
//            m_address[i * 2]     = temp.split[0];
//            m_address[i * 2 + 1] = temp.split[1];
//        }
    }
}

IPAddress::~IPAddress()
{
}

String IPAddress::toString() const
{
    if (! m_isIPv6)
    {
        String s ((int) m_address[0]);
        
        for (int i = 1; i < 4; ++i)
            s << '.' << (int) m_address[i];
        
        return s;
    }
    
    String result;
    ByteUnion temp;
    
    temp.split[0] = m_address[0];
    temp.split[1] = m_address[1];
    
    Number::appendHex(result, temp.combined);
    
    for (int i = 1; i < 8; ++i)
    {
        temp.split[0] = m_address[i * 2];
        temp.split[1] = m_address[i * 2 + 1];
        
        result << ':';
        Number::appendHex(result, temp.combined);
    }
    
    return result;
//    return getFormattedAddress (result);
}

bool IPAddress::operator==(const IPAddress &other) const noexcept
{
    for (int i = 0; i < (m_isIPv6 ? 16 : 4); ++i)
        if (m_address[i] != other.m_address[i])
            return false;
    
    return true;
}

bool IPAddress::operator!=(const IPAddress &other) const noexcept
{
    return ! operator==(other);
}

CUTE_NS_END
