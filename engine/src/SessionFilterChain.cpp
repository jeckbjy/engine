//! Server
#include "Cute/SessionFilterChain.h"
#include "Cute/SessionContext.h"
#include "Cute/SessionFilter.h"
#include "Cute/Session.h"

CUTE_NS_BEGIN

SessionFilterChain::SessionFilterChain()
{
    
}

SessionFilterChain::~SessionFilterChain()
{
    foreach(SessionFilter* filter, m_inbounds)
    {
        filter->release();
    }
    
    m_inbounds.clear();
    
    foreach(SessionFilter* filter, m_outbounds)
    {
        filter->release();
    }
    
    m_outbounds.clear();
}

void SessionFilterChain::addFront(SessionFilter *filter, int groups)
{
    if((groups & FILTER_INBOUND) != 0)
    {
        filter->retain();
        m_inbounds.push_front(filter);
    }
    
    if((groups & FILTER_OUTBOUND) != 0)
    {
        filter->retain();
        m_outbounds.push_front(filter);
    }
}

void SessionFilterChain::addLast(SessionFilter *filter, int groups)
{
    if((groups & FILTER_INBOUND) != 0)
    {
        filter->retain();
        m_inbounds.push_back(filter);
    }
    
    if((groups & FILTER_OUTBOUND) != 0)
    {
        filter->retain();
        m_outbounds.push_back(filter);
    }
}

void SessionFilterChain::doSend(Session* sess, Object* msg)
{
    SessionContext context(sess);
    context.setSendMessage(msg);
    for(FilterList::reverse_iterator itor = m_outbounds.rbegin(); itor != m_outbounds.rend(); ++itor)
    {
        (*itor)->doSend(&context);
        if(context.isStop())
            break;
    }
    
    // read send
    if(!context.isStop())
    {
        sess->doSend(context.getSendBuffer());
    }
}

void SessionFilterChain::doConnect(Session* sess, const SocketAddress& addr)
{
    SessionContext context(sess);
    context.setConnectAddress(addr);
    for(auto itor = m_outbounds.rbegin(); itor != m_outbounds.rend(); ++itor)
    {
        (*itor)->doConnect(&context);
        if(context.isStop())
            break;
    }
}

void SessionFilterChain::doClose(Session *sess)
{
    SessionContext context(sess);
    for(auto itor = m_outbounds.rbegin(); itor != m_outbounds.rend(); ++itor)
    {
        (*itor)->doClose(&context);
        if(context.isStop())
            break;
    }
}

void SessionFilterChain::onRead(Session* sess)
{
    SessionContext context(sess);
    for(FilterList::iterator itor = m_inbounds.begin(); itor != m_inbounds.end(); ++itor)
    {
        (*itor)->onRead(&context);
        if(context.isStop())
            break;
    }
}

void SessionFilterChain::onSend(Session* sess)
{
    SessionContext context(sess);
    for(FilterList::iterator itor = m_inbounds.begin(); itor != m_inbounds.end(); ++itor)
    {
        (*itor)->onSend(&context);
        if(context.isStop())
            break;
    }
}

void SessionFilterChain::onConnect(Session* sess)
{
    SessionContext context(sess);
    for(FilterList::iterator itor = m_inbounds.begin(); itor != m_inbounds.end(); ++itor)
    {
        (*itor)->onConnect(&context);
        if(context.isStop())
            break;
    }
}

void SessionFilterChain::onAccept(Session* sess, SocketChannel* channel)
{
    SessionContext context(sess);
    context.setAcceptChannel(channel);
    for(FilterList::iterator itor = m_inbounds.begin(); itor != m_inbounds.end(); ++itor)
    {
        (*itor)->onAccept(&context);
        if(context.isStop())
            break;
    }
}

void SessionFilterChain::onError(Session* sess, error_t code, const String& text)
{
    SessionContext context(sess);
    context.setErrorCode(code);
    context.setErrorText(text);
    for(FilterList::iterator itor = m_inbounds.begin(); itor != m_inbounds.end(); ++itor)
    {
        (*itor)->onError(&context);
        if(context.isStop())
            break;
    }
}

CUTE_NS_END
