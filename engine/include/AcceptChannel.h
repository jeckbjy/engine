#pragma once
#include "Channel.h"
#include "Socket.h"

CU_NS_BEGIN

class CU_API AcceptChannel : public Channel
{
public:
	AcceptChannel(IOService* service);
	~AcceptChannel();

	void accept();
	void listen(const SocketAddress& addr);
	virtual void perform(IOOperation* op);
	virtual void completed(socket_t sock);

	handle_t handle() const { return (handle_t)m_sock.native(); }

protected:
	Socket	m_sock;
	SocketAddress m_addr;
};

CU_NS_END
