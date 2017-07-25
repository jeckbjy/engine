//! Text
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"
#include <limits>
#include <cmath>

CUTE_NS_BEGIN

// binary numbers are supported, thus 64 (bits) + 1 (string terminating zero)
#define CUTE_MAX_INT_STRING_LEN 65
// value from strtod.cc (double_conversion::kMaxSignificantDecimalDigits)
#define CUTE_MAX_FLT_STRING_LEN 780

#define CUTE_FLT_INF "inf"
#define CUTE_FLT_NAN "nan"
#define CUTE_FLT_EXP 'e'

// 8进制和16进制没有负数
class Numeric
{
public:
    enum Type
    {
        TYPE_NONE,
        TYPE_SINT,
        TYPE_UINT,
        TYPE_FLOAT,
    };
    
    struct Real
    {
        int type;
        union
        {
            int64	vi;
            uint64	vu;
            double	vd;
        };
    };
    
public:
    template<typename T>
    static bool formatDec(char* buffer, size_t& size, T value, int width, char fill, char thSep);
    template<typename T>
    static bool formatHex(char* buffer, size_t& size, T value, int width, char fill, bool prefix);
    template<typename T>
    static bool formatOct(char* buffer, size_t& size, T value, int width, char fill, bool prefix);
    
    static const char* parseNumber(Real& result, const char* buffer);
    
private:
    template<typename T>
    static bool formatInteger(char* buffer, size_t& size, T value, short base, int width = -1, char fill = ' ', char thSep = 0, bool prefix = false);
};

template<typename T>
bool Numeric::formatDec(char *buffer, size_t &size, T value, int width, char fill, char thSep)
{
    if(value < 0)
    {
        bool result = Numeric::formatInteger(buffer + 1, --size, -value , 10, width, fill, thSep);
        buffer[0] = '-';
        size += 1;
        return result;
    }
    else
    {
        return Numeric::formatInteger(buffer, size, value, 10, width, fill, thSep);
    }
    
}

template<typename T>
bool Numeric::formatHex(char *buffer, size_t &size, T value, int width, char fill, bool prefix)
{
    return Numeric::formatInteger(buffer, size, (uint64)value, 16, width, fill, 0, prefix);
}

template<typename T>
bool Numeric::formatOct(char *buffer, size_t &size, T value, int width, char fill, bool prefix)
{
    return Numeric::formatInteger(buffer, size, (uint64)value, 8, width, fill, 0, prefix);
}

// result:需要保证内存足够大
template<typename T>
bool Numeric::formatInteger(char *result, size_t &size, T value, short base, int width, char fill, char thSep, bool prefix)
{
    if (base < 2 || base > 0x10)
    {
        *result = '\0';
        return false;
    }
    
    char* ptr = result;
    int thCount = 0;
    T tmpVal;
    do
    {
        tmpVal = value;
        value /= base;
        *ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
        if (thSep && (base == 10) && (++thCount == 3))
        {
            *ptr++ = thSep;
            thCount = 0;
        }
    } while (value);
    
    if ('0' == fill)
    {
        if (prefix && base == 010) --width;
        if (prefix && base == 0x10) width -= 2;
        while ((ptr - result) < width) *ptr++ = fill;
    }
    
    if (prefix && base == 010) *ptr++ = '0';
    else if (prefix && base == 0x10)
    {
        *ptr++ = 'x';
        *ptr++ = '0';
    }
    
    if ('0' != fill)
    {
        while ((ptr - result) < width) *ptr++ = fill;
    }
    
    size = ptr - result;
    *ptr-- = '\0';
    
    char* ptrr = result;
    char tmp;
    while(ptrr < ptr)
    {
        tmp    = *ptr;
        *ptr--  = *ptrr;
        *ptrr++ = tmp;
    }
    
    return true;
}

CUTE_NS_END
