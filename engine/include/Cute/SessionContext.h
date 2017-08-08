#pragma once
#include "Cute/Foundation.h"
#include "Cute/Variant.h"
#include "Cute/BufferList.h"
#include "Cute/SocketAddress.h"

CUTE_NS_BEGIN

// 用于Filter之间数据传递
// use Object or Void or Any to save data?
class CUTE_CORE_API SessionContext : public TObject<SessionContext>
{
public:
    SessionContext(Session* sess);
    
    bool hasContextValue(const String& key) const;
    bool getContextValue(const String& key, Variant& value) const;
    void setContextValue(const String& key, Variant& value);
    
    bool hasSessionValue(const String& key) const;
    bool getSessionValue(const String& key, Variant& value) const;
    void setSessionValue(const String& key, Variant& value);
    
//    void setInternal(const Any& value);
//    bool getInternal();
    
    void            setSendMessage(Object* msg);
    Object*         getSendMessage() const;
    
    void            setConnectAddress(const SocketAddress& addr);
    const SocketAddress& getConnectAddress() const;
    
    void            setAcceptChannel(SocketChannel* channel);
    SocketChannel*  getAcceptChannel();
    
    BufferList&     getReadBuffer();
    BufferList&     getSendBuffer();
    
    void            setErrorCode(error_t code);
    void            setErrorText(const String& text);
    error_t         getErrorCode() const;
    const String&   getErrorText() const;
    
    void stop();
    bool isStop();
    
private:
    // internal data, store pointer for filter!
    union InternalData
    {
        Object*                 message;
        SocketChannel*          channel;
        const SocketAddress*    address;
    };
    
    typedef Map<String, Variant> VariantMap;
    bool            m_stop;
    Session*        m_session;
    VariantMap      m_values;
    BufferList      m_buffer;
//    Any             m_internal;
    InternalData    m_internal;
    String          m_errorText;
    error_t         m_errorCode;
};

CUTE_NS_END
