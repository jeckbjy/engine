#pragma once
#include "Cute/IOChannel.h"
#include "Cute/BufferList.h"
#include "Cute/SocketDefs.h"
#include "Cute/SocketAddress.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

class SocketChannel;

class CUTE_CORE_API SocketListener
{
public:
	virtual ~SocketListener(){}
	virtual void fireRead(SocketChannel* channel) = 0;
	virtual void fireWrite(SocketChannel* channel) = 0;
	virtual void fireConnect(SocketChannel* channel) = 0;
	virtual void fireError(SocketChannel* channel) = 0;
};

class CUTE_CORE_API SocketChannel : public IOChannel
{
public:
	SocketChannel(IOLoop* loop);
	virtual ~SocketChannel();

	virtual void connect(const SocketAddress& addr) = 0;
	virtual void send(const BufferList& data) = 0;
	virtual void recv() = 0;
	virtual void shutdown(int how = SHUT_RDWR) = 0;

	virtual SocketAddress localAddress() const = 0;
	virtual SocketAddress peerAddress() const = 0;
	virtual BufferList&	getReader() = 0;

	void setListener(SocketListener* listener);
	SocketListener* getListener() const { return m_listener; }

protected:
	SocketListener*	m_listener;
};

CUTE_NS_END
