#pragma once
#include "Cute/RefPtr.h"
#include "Cute/SocketChannel.h"
#include "Cute/SessionFilterChain.h"
#include "Cute/Variant.h"

CUTE_NS_BEGIN

enum SessionType
{
    SESS_ANY,
    SESS_CLIENT,
    SESS_GAME,
    SESS_WORLD,
    SESS_WEB,
};

/*
 1:基于Filter设计
 2:可持有一个Object类型Data和多个kv数据
 */
class CUTE_CORE_API Session : public TObject<Session>, public SocketListener
{
public:
    Session(SocketChannel* socket, uint32 id, uint32 type = 0);
    virtual ~Session();
    
    uint32	getID() const { return m_id; }
    
    uint32	getType() const { return m_type; }
    void	setType(uint32 type) { m_type = type; }
    
    Object*	getData() { return m_data; }
    void	setData(Object* data) { m_data = data; }
    
    void    setFilterChain(SessionFilterChain* filters) { m_filters = filters; }
    
    bool    hasValue(const String& key) const;
    bool    getValue(const String& key, Variant& value) const;
    void    setValue(const String& key, const Variant& value);
    
    bool	isActive() const;
    bool	isClosing() const;
    void	setClosing();
    
    void    send(Object* data);
    void    sendBytes(const BufferList& data);
    void	connect(const SocketAddress& addr);
    void	reconnect();
    void	shutdown(int how = SHUT_RD);
    void	close();
    
    const SocketAddress& getPeer() const { return m_peer; }
    SocketChannel*       getChannel() { return m_socket.get(); }
    
protected:
    void doSend(const BufferList& data);
    void doConnect(const SocketAddress& addr);
    void doClose();
    
    virtual void fireRead(SocketChannel* channel);
    virtual void fireWrite(SocketChannel* channel);
    virtual void fireConnect(SocketChannel* channel);
    virtual void fireError(SocketChannel* channel);
    
protected:
    friend class SessionFilterChain;
    typedef RefPtr<SessionFilterChain>  FilterChainPtr;
    typedef RefPtr<SocketChannel>       SocketPtr;
    typedef Map<String, Variant>        VariantMap;
    SocketPtr		m_socket;
    SocketAddress	m_peer;
    uint32			m_id;
    uint32			m_type;
    Object*			m_data;
    FilterChainPtr  m_filters;
    VariantMap      m_values;
};

CUTE_NS_END

//#pragma once
//#include "Cute/RefPtr.h"
//#include "Cute/SocketChannel.h"
//#include "Cute/EventBase.h"
//#include "Cute/Protocal.h"
//
//CUTE_NS_BEGIN
//
//enum SessionType
//{
//	SESS_ANY,
//	SESS_CLIENT,
//	SESS_GAME,
//	SESS_WORLD,
//	SESS_WEB,
//};
//
//class CUTE_CORE_API Session : public Object, public SocketListener
//{
//	DECLARE_RTTI(Session, Object, OID_ANY)
//public:
//	Session(Protocal* protocal, SocketChannel* socket, uint32 id, uint32 type = 0);
//	virtual ~Session();
//
//	uint32	getID() const { return m_id; }
//	uint32	getType() const { return m_type; }
//	void	setType(uint32 type) { m_type = type; }
//	void	setData(Object* data) { m_data = data; }
//	Object*	getData() { return m_data; }
//	void	setProtocal(Protocal* protocal) { m_protocal = protocal; }
//
//	const SocketAddress& getPeer() const { return m_peer; }
//	SocketChannel*       getChannel() { return m_socket.get(); }
//
//	bool	isActive() const;
//	bool	isClosing() const;
//	void	setClosing();
//
//	void	send(const BufferList& data);
//	void	connect(const SocketAddress& addr);
//	void	reconnect();
//	void	shutdown(int how = SHUT_RD);
//	void	close();
//	void	addPending(int pending);
//
//protected:
//	virtual void fireRead(SocketChannel* channel);
//	virtual void fireWrite(SocketChannel* channel);
//	virtual void fireConnect(SocketChannel* channel);
//	virtual void fireError(SocketChannel* channel);
//
//protected:
//    typedef SessionFilterChain FilterChain;
//	typedef RefPtr<SocketChannel> SocketPtr;
//	SocketPtr		m_socket;
//	SocketAddress	m_peer;
//	uint32			m_id;
//	uint32			m_type;
//	Object*			m_data;
//	Protocal*		m_protocal;
//    FilterChain*    m_filters;
//};
//
//CUTE_NS_END
