//! Network/Core
#include "Cute/MACAddress.h"
#include "Cute/Number.h"
#include <net/if.h>

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOS)

static void addMACAddress(Vector<MACAddress> &results, const MACAddress& ma)
{
    if(!ma.isNUll())
        results.push_back(ma);
}

static void getMacAddressViaAdaptersAddresses(Vector<MACAddress> &results)
{
    SharedLibrary dll("iphlpapi.dll");
    CUTE_LOAD_FUNCTION(dll, GetAdaptersAddresses, getAdaptersAddresses, DWORD, (ULONG, ULONG, PVOID, PIP_ADAPTER_ADDRESSES, PULONG));
    if(getAdaptersAddresses == NULL)
        return;
    
    HeapBlock<IP_ADAPTER_ADDRESSES> addrs(1);
    ULONG len= sizeof(IP_ADAPTER_ADDRESSES)
    if(getAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, addrs, &len) == ERROR_BUFFER_OVERFLOW)
    {
        addrs.malloc(len, 1);
        if(getAdaptersAddresses (AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, addrs, &len) == NO_ERROR)
            return;
    }
    
    for (PIP_ADAPTER_ADDRESSES adapter = addressesHelper.adaptersAddresses; adapter != nullptr; adapter = adapter->Next)
    {
        if (adapter->PhysicalAddressLength >= 6)
            addMACAddress(results, MACAddress (adapter->PhysicalAddress));
    }
}

static void getMacAddressViaNetBios(Vector<MACAddress> &results)
{
    SharedLibrary dll("netapi32.dll");
    CUTE_LOAD_FUNCTION(dll, Netbios, NetbiosCall, UCHAR, (PNCB));
    if(NetbiosCall == NULL)
        return;
    
    LANA_ENUM enums = { 0 };
    
    {
        NCB ncb = { 0 };
        ncb.ncb_command = NCBENUM;
        ncb.ncb_buffer = (unsigned char*) &enums;
        ncb.ncb_length = sizeof (LANA_ENUM);
        NetbiosCall (&ncb);
    }
    
    for (int i = 0; i < enums.length; ++i)
    {
        NCB ncb2 = { 0 };
        ncb2.ncb_command = NCBRESET;
        ncb2.ncb_lana_num = enums.lana[i];
        
        if (NetbiosCall (&ncb2) == 0)
        {
            NCB ncb = { 0 };
            memcpy (ncb.ncb_callname, "*                   ", NCBNAMSZ);
            ncb.ncb_command = NCBASTAT;
            ncb.ncb_lana_num = enums.lana[i];
            
            struct ASTAT
            {
                ADAPTER_STATUS adapt;
                NAME_BUFFER    NameBuff [30];
            };
            
            ASTAT astat;
            zerostruct (astat);
            ncb.ncb_buffer = (unsigned char*) &astat;
            ncb.ncb_length = sizeof (ASTAT);
            
            if (NetbiosCall (&ncb) == 0 && astat.adapt.adapter_type == 0xfe)
                addMACAddress (results, MACAddress (astat.adapt.adapter_address));
        }
    }
    // end
}

void MACAddress::findAllAddresses(Vector<MACAddress> &results)
{
    getMacAddressViaAdaptersAddresses(result);
    getMacAddressViaNetBios(result);
}

#elif defined(CUTE_OS_FAMILY_LINUX)

void MACAddress::findAllAddresses(Vector<MACAddress> &results)
{
    const int s = socket (AF_INET, SOCK_DGRAM, 0);
    if (s != -1)
    {
        struct ifaddrs* addrs = nullptr;
        
        if (getifaddrs (&addrs) != -1)
        {
            for (struct ifaddrs* i = addrs; i != nullptr; i = i->ifa_next)
            {
                struct ifreq ifr;
                strcpy (ifr.ifr_name, i->ifa_name);
                ifr.ifr_addr.sa_family = AF_INET;
                
                if (ioctl (s, SIOCGIFHWADDR, &ifr) == 0)
                {
                    MACAddress ma ((const uint8*) ifr.ifr_hwaddr.sa_data);
                    
                    if (! ma.isNull())
                        results.push_back(ma);
//                        results.addIfNotAlreadyThere (ma);
                }
            }
            
            freeifaddrs (addrs);
        }
        
        close (s);
    }
}
#elif defined(CUTE_OS_FAMILY_BSD)

void MACAddress::findAllAddresses(Vector<MACAddress> &results)
{
    
}

#endif

MACAddress::MACAddress() noexcept
{
    memset(m_address, 0, sizeof(m_address));
}

MACAddress::MACAddress(const uint8 bytes[6]) noexcept
{
    memcpy(m_address, bytes, sizeof(m_address));
}

MACAddress::MACAddress(const String& addr) noexcept
{
    
}

MACAddress::MACAddress(const MACAddress& other) noexcept
{
    memcpy(m_address, other.m_address, sizeof(m_address));
}

MACAddress& MACAddress::operator=(const MACAddress &other) noexcept
{
    memcpy(m_address, other.m_address, sizeof(m_address));
    return *this;
}

String MACAddress::toString(const String& separator) const
{
    String result;
    
    for (size_t i = 0; i < sizeof (m_address); ++i)
    {
        Number::appendHex(result, 2, false);
//        result << String::toHexString ((int) address[i]).paddedLeft ('0', 2);
        
        if (i < sizeof (m_address) - 1)
            result << separator;
    }
    
    return result;
}

int64 MACAddress::toInt64() const noexcept
{
    int64 n = 0;
    
    for (int i = (int) sizeof (m_address); --i >= 0;)
        n = (n << 8) | m_address[i];
        
    return n;
}

bool MACAddress::isNull() const noexcept
{
    return toInt64() == 0;
}

bool MACAddress::operator== (const MACAddress& other) const noexcept
{
    return memcmp (m_address, other.m_address, sizeof (m_address)) == 0;
}

bool MACAddress::operator!= (const MACAddress& other) const noexcept
{
    return ! operator== (other);
}

CUTE_NS_END
