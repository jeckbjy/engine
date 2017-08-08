#pragma once
#include "Cute/SessionFilter.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

class CUTE_CORE_API FrameFilter : public SessionFilter
{
public:
    virtual ~FrameFilter(){}
    
    virtual void doSend(SessionContext* ctx);
    virtual void onRead(SessionContext* ctx);
    
protected:
    virtual void encode(BufferList& buffer) = 0;
    virtual void decode(BufferList& buffer) = 0;
};

class CUTE_CORE_API LengthedFrameFilter : public FrameFilter
{
public:
    void encode(BufferList& buffer);
    void decode(BufferList& buffer);
};

class CUTE_CORE_API DelimiterFrameFilter : public FrameFilter
{
public:
    void encode(BufferList& buffer);
    void decode(BufferList& buffer);
    
protected:
    String m_delimiter;
};

CUTE_NS_END
