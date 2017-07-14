#pragma core
#include "Cute/Platform.h"

CUTE_NS_BEGIN

class CUTE_CORE_API MessagePump
{
public:
    static void dispatch();
    static void quit();
};

CUTE_NS_END
