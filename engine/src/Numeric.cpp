//! Text
#include "Cute/Numeric.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

const char* Numeric::parseNumber(Real& result, const char* buffer)
{
    result.type = TYPE_NONE;
    
    const char* p = buffer;
    if (*p == 0)
        return p;
    
    // check negative
    bool isNegative = false;
    if ('-' == *p)
    {
        isNegative = true;
        ++p;
        if (*p == 0)
            return p;
    }
    
    bool isDouble = false;
    uint64 integer = 0;
    int    exponent = 0;
    char c;
    char digit;
    // integral part
    while (*p != 0)
    {
        c = *p;
        if (c < '0' || c > '9')
            break;
        
        ++p;
        digit = c - '0';
        integer = integer * 10 + digit;
    }
    
    // fractional part
    c = *p;
    if (c == '.')
    {
        isDouble = true;
        ++p;
        while (*p != 0)
        {
            c = *p;
            if (c < '0' || c > '9')
                break;
            ++p;
            digit = c - '0';
            integer = integer * 10 + digit;
            --exponent;
        }
    }
    
    // exponential part
    if (*p == 'e' || *p == 'E')
    {
        isDouble = true;
        ++p;
        bool isNegativeExponent = false;
        // error
        c = *p;
        if (c == 0)
        {
            return p;
        }
        else if (c == '-')
        {
            isNegativeExponent = true;
            ++p;
        }
        else if (c == '+')
        {
            ++p;
        }
        
        int exp = 0;
        while (*p != 0)
        {
            c = *p;
            if (c < '0' || c > '9')
                break;
            ++p;
            digit = c - '0';
            exp = exp * 10 + digit;
        }
        
        exponent += (isNegativeExponent ? -exp : exp);
    }
    
    if (isDouble)
    {
        double d = (double)integer;
        if (isNegative)
            d = -d;
        if (exponent != 0)
            d = d * Math::pow10(exponent);
        
        result.type = TYPE_FLOAT;
        result.vd = d;
    }
    else if (isNegative)
    {
        result.type = TYPE_SINT;
        result.vi = -(int64)integer;
    }
    else
    {
        result.type = TYPE_UINT;
        result.vu = integer;
    }
    
    return p;
}

CUTE_NS_END
