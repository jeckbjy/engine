//! Server/Filter
#include "Cute/EncryptFilter.h"
#include "Cute/SessionContext.h"

CUTE_NS_BEGIN

const StringID EncryptFilter::RC4_SEND_KEY = "RC4_SEND_KEY";
const StringID EncryptFilter::RC4_READ_KEY = "RC4_READ_KEY";

void EncryptFilter::setup()
{
    // random read and write key
}

void EncryptFilter::clean()
{
    // erase key
}

void EncryptFilter::doSend(SessionContext *ctx)
{
//    String key;
//    bool result = ctx->getSessionValue("RC4_SEND_KEY", key);
//    if(!result)
//        return;
//    
//    BufferList& buffer = ctx->getSendBuffer();
    // encrypt
}

void EncryptFilter::onRead(SessionContext *ctx)
{
    // 解密
}

CUTE_NS_END
