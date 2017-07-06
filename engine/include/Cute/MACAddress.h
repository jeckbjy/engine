#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API MACAddress
{
public:
    static void findAllAddresses(Vector<MACAddress>& results);
    
    MACAddress() noexcept;
    MACAddress(const uint8 bytes[6]) noexcept;
    MACAddress(const String& address) noexcept;
    MACAddress(const MACAddress& other) noexcept;
    MACAddress& operator=(const MACAddress&) noexcept;
    
    const uint8* getBytes() const noexcept;
    
    /** Returns a dash-separated string in the form "11-22-33-44-55-66" */
    String toString() const;
    String toString(const String& separator) const;
    /** Returns the address in the lower 6 bytes of an int64.
     
     This uses a little-endian arrangement, with the first byte of the address being
     stored in the least-significant byte of the result value.
     */
    int64  toInt64() const noexcept;
    
    /** Returns true if this address is null (00-00-00-00-00-00). */
    bool isNull() const noexcept;
    
    bool operator== (const MACAddress&) const noexcept;
    bool operator!= (const MACAddress&) const noexcept;
    
private:
    uint8 m_address[6];
};

inline const uint8* MACAddress::getBytes() const noexcept
{
    return m_address;
}

inline String MACAddress::toString() const
{
    return toString("-");
}

CUTE_NS_END
