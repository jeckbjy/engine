#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Util
{
public:
    static bool convert(const char* from, int8& to);
    static bool convert(const char* from, int16& to);
    static bool convert(const char* from, int32& to);
    static bool convert(const char* from, int64& to);
    static bool convert(const char* from, uint8& to);
    static bool convert(const char* from, uint16& to);
    static bool convert(const char* from, uint32& to);
};

CUTE_NS_END
