#pragma once
#include "Cute/SessionFilter.h"
#include "Cute/StringID.h"

CUTE_NS_BEGIN

// 加密：RC4加密
class CUTE_CORE_API EncryptFilter : public SessionFilter
{
public:
    static const StringID RC4_SEND_KEY;
    static const StringID RC4_READ_KEY;
    
    void setup();
    void clean();
    
    void doSend(SessionContext* ctx) OVERRIDE;
    void onRead(SessionContext* ctx) OVERRIDE;
};

CUTE_NS_END
