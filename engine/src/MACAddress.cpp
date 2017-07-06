//! Network/Core
#include "Cute/MACAddress.h"
#include "Cute/Number.h"

CUTE_NS_BEGIN

void MACAddress::findAllAddresses(Vector<Cute::MACAddress> &results)
{
    // todo:
}

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
