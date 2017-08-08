#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

enum FilterGroup
{
    FILTER_INBOUND  = 0x01,
    FILTER_OUTBOUND = 0x02,
    FILTER_BOTH     = 0x03,
};

class CUTE_CORE_API SessionFilterChain : public TObject<SessionFilterChain>
{
public:
    SessionFilterChain();
    ~SessionFilterChain();
    
    void addFront(SessionFilter* filter, int groups);
    void addLast(SessionFilter* filter, int groups);
    
    void doSend(Session* sess, Object* msg);
    void doConnect(Session* sess, const SocketAddress& addr);
    void doClose(Session* sess);
    
    void onRead(Session* sess);
    void onSend(Session* sess);
    void onConnect(Session* sess);
    void onAccept(Session* sess, SocketChannel* channel);
    void onError(Session* sess, error_t code, const String& text = "");
    
private:
    typedef std::list<SessionFilter*> FilterList;
    FilterList m_inbounds;
    FilterList m_outbounds;
};

CUTE_NS_END
