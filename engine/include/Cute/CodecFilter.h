#pragma once
#include "Cute/SessionFilter.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

class CUTE_CORE_API CodecFilter : public SessionFilter
{
public:
    virtual void doSend(SessionContext* ctx);
    virtual void onRead(SessionContext* ctx);
    
    virtual void encode(BufferList& buffer) = 0;
    virtual bool decode(BufferList& buffer) = 0;
};

class CUTE_CORE_API PacketCodecFilter : public SessionFilter
{
public:
    void doSend(SessionContext* ctx) { encode(ctx); }
    void onRead(SessionContext* ctx) { decode(ctx); }
    
    void encode(SessionContext* ctx);
    void decode(SessionContext* ctx);
};

// 纯文本
class CUTE_CORE_API TextCodecFilter : public SessionFilter
{
public:
};

class CUTE_CORE_API JsonCodecFilter : public SessionFilter
{
public:
    void doSend(SessionContext* ctx) { encode(ctx); }
    void onRead(SessionContext* ctx) { decode(ctx); }
    
    void encode(SessionContext* ctx);
    void decode(SessionContext* ctx);
};

//class CUTE_CORE_API ProtobufCodecFilter : public SessionFilter
//{
//    
//};

CUTE_NS_END
