#pragma once
#include "AcceptChannel.h"

CU_NS_BEGIN

class CU_API Acceptor
{
public:
	Acceptor(IOService* service, uint type = 0);
	~Acceptor();

	void close() { m_channel->close(); }
	void completed(error_t ec, socket_t sock);

	void setType(uint type) { m_type = type; }
	uint getType() const { return m_type; }

	AcceptChannel* getChannel() { return m_channel; }
	void accept(){ m_channel->accept(); }
	void listen(const SocketAddress& addr, int backlog = 64);

private:
	uint m_type;
	AcceptChannel* m_channel;
};

inline void Acceptor::listen(const SocketAddress& addr, int backlog /* = 64 */)
{
	m_channel->listen(addr, backlog);
}

CU_NS_END