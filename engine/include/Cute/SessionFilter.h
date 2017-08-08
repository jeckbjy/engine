//! Server
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/*
 * 1:Filter:只含有算法，不能含有数据，如果需要临时数据，使用ContextValue，如果需要持久数据，使用SessionValue
 * 2:Filter包含两种流向，inbound(onRead,onError)和outbound(send,connect,close)
 * 3:inbound:按照注册顺序执行，outbound:按照注册顺序，反向执行
 * 4:如何线程安全的执行Filter,onXxxxx通常都是在网络线程，需要派发到逻辑线程中执行
 */
class CUTE_CORE_API SessionFilter : public TObject<SessionFilter>
{
public:
    virtual ~SessionFilter(){}
    
    virtual void doSend(SessionContext* ctx){}
    virtual void doConnect(SessionContext* ctx){}
    virtual void doClose(SessionContext* ctx){}
    
    virtual void onRead(SessionContext* ctx){}
    virtual void onSend(SessionContext* ctx){}
    virtual void onConnect(SessionContext* ctx){}
    virtual void onAccept(SessionContext* ctx){}
    virtual void onError(SessionContext* ctx){}
};

CUTE_NS_END
