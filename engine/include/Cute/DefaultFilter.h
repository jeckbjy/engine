#pragma once
#include "Cute/SessionFilter.h"

CUTE_NS_BEGIN

// 默认方式:集合了Frame,Encrypt, Codec, Execution
class CUTE_CORE_API DefaultFilter : public SessionFilter
{
public:
    void doSend(SessionContext* ctx);
    void onRead(SessionContext* ctx);
};

CUTE_NS_END
