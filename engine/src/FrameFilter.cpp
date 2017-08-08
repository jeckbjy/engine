//! Server/Filter
#include "Cute/FrameFilter.h"
#include "Cute/SessionContext.h"

CUTE_NS_BEGIN

void FrameFilter::doSend(SessionContext *ctx)
{
    BufferList& buffer = ctx->getSendBuffer();
    encode(buffer);
}

void FrameFilter::onRead(SessionContext *ctx)
{
    BufferList& buffer = ctx->getReadBuffer();
    decode(buffer);
}

void LengthedFrameFilter::encode(BufferList &buffer)
{
    // 两个字节：65535
    uint16 length = (uint16)buffer.size();
//    char data[10];
    buffer.seek(0, SEEK_SET);
    buffer.writeLittle(length);
}

void LengthedFrameFilter::decode(BufferList &buffer)
{
//    buffer.peek(<#void *data#>, <#size_t len#>)
}

void DelimiterFrameFilter::encode(BufferList &buffer)
{
//    size_t pos = buffer.find(m_delimiter.c_str());
    buffer.write(m_delimiter.c_str(), m_delimiter.size());
}

void DelimiterFrameFilter::decode(BufferList &buffer)
{
//    size_t pos = buffer.find(m_delimiter.c_str(), 0);
    
}

CUTE_NS_END
