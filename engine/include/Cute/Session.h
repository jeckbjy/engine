#pragma once
#include "Cute/RefPtr.h"
#include "Cute/SocketChannel.h"
#include "Cute/LogicEvent.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Session : public Object, public SocketListener
{
	DECLARE_RTTI(Session, Object, OID_ANY)
public:
	Session(SocketChannel* socket, uint32 id, uint32 type = 0);
	virtual ~Session();

	uint32	getID() const { return m_id; }
	uint32	getType() const { return m_type; }
	void	setType(uint32 type) { m_type = type; }
	void	setData(Object* data) { m_data = data; }
	Object*	getData() { return m_data; }

	const SocketAddress& getPeer() const { return m_peer; }
	SocketChannel* getSocket() { return m_socket.get(); }

	bool	isActive() const;
	bool	isClosing() const;
	void	setClosing();

	void	send(const BufferList& data);
	void	connect(const SocketAddress& addr);
	void	reconnect();
	void	shutdown(int how = SHUT_RD);
	void	close();

protected:
	virtual void fireRead(SocketChannel* channel);
	virtual void fireWrite(SocketChannel* channel);
	virtual void fireConnect(SocketChannel* channel);
	virtual void fireError(SocketChannel* channel);

protected:
	typedef RefPtr<SocketChannel> SocketPtr;
	SocketPtr		m_socket;
	uint32			m_id;
	uint32			m_type;
	Object*			m_data;
	SocketAddress	m_peer;
};

CUTE_NS_END
