//! Server
#include "Cute/SessionContext.h"
#include "Cute/Session.h"

CUTE_NS_BEGIN

SessionContext::SessionContext(Session* sess)
    : m_session(sess)
    , m_stop(false)
{
}

bool SessionContext::hasContextValue(const String &key) const
{
    return m_values.find(key) != m_values.end();
}

bool SessionContext::getContextValue(const String &key, Variant &value) const
{
    VariantMap::const_iterator itor = m_values.find(key);
    if(itor != m_values.end())
    {
        value = itor->second;
        return true;
    }
    
    return false;
}

void SessionContext::setContextValue(const String &key, Variant &value)
{
    m_values[key] = value;
}

bool SessionContext::hasSessionValue(const String &key) const
{
    return m_session->hasValue(key);
}

bool SessionContext::getSessionValue(const String &key, Variant &value) const
{
    return m_session->getValue(key, value);
}

void SessionContext::setSessionValue(const String &key, Variant &value)
{
    m_session->setValue(key, value);
}

void SessionContext::setSendMessage(Object *msg)
{
    m_internal.message = msg;
}

Object* SessionContext::getSendMessage() const
{
    return m_internal.message;
}

void SessionContext::setConnectAddress(const SocketAddress &addr)
{
    m_internal.address = &addr;
}

const SocketAddress& SessionContext::getConnectAddress() const
{
    return *(m_internal.address);
}

void SessionContext::setAcceptChannel(SocketChannel *channel)
{
    m_internal.channel = channel;
}

SocketChannel* SessionContext::getAcceptChannel()
{
    return m_internal.channel;
}

BufferList& SessionContext::getReadBuffer()
{
    return m_session->getChannel()->getReader();
}

BufferList& SessionContext::getSendBuffer()
{
    return m_buffer;
}

void SessionContext::setErrorCode(error_t code)
{
    m_errorCode = code;
}

void SessionContext::setErrorText(const String &text)
{
    m_errorText = text;
}

error_t SessionContext::getErrorCode() const
{
    return m_errorCode;
}

const String& SessionContext::getErrorText() const
{
    return m_errorText;
}

void SessionContext::stop()
{
    m_stop = true;
}

bool SessionContext::isStop()
{
    return m_stop;
}

CUTE_NS_END
