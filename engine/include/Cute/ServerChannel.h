#pragma once
#include "Cute/SocketAddress.h"
#include "Cute/IOChannel.h"

CUTE_NS_BEGIN

class SocketChannel;
class ServerChannel;
class CUTE_CORE_API ServerListener
{
public:
	virtual ~ServerListener(){}
	virtual void fireAccept(ServerChannel* listener, SocketChannel* channel) = 0;
};

class CUTE_CORE_API ServerChannel : public IOChannel
{
	DECLARE_RTTI(ServerChannel, IOChannel, OID_ANY);
public:
	ServerChannel(IOLoop* loop);
	virtual ~ServerChannel();

	virtual void listen(const SocketAddress& addr) = 0;

	void setListener(ServerListener* listener);
	ServerListener* getListener() const { return m_listener; }

	void	setID(uint32 id) { m_id = id; }
	uint32	getID() const { return m_id; }

	void	setType(uint32_t type) { m_type = type; }
	uint32	getType() const { return m_type; }

protected:
	ServerListener* m_listener;
	uint32		m_id;		// 唯一id
	uint32		m_type;		// 类型标识
};

CUTE_NS_END
