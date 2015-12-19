#pragma once
#include "Channel.h"
#include "Socket.h"

CU_NS_BEGIN

class CU_API AcceptChannel : public Channel
{
public:
	AcceptChannel(EventLoop* loop);
	~AcceptChannel();

	void accept();
	void listen(const SocketAddress& addr);
	void perform(IOOperation* op);

	handle_t handle() const { return (handle_t)m_sock.native(); }

protected:
	Socket m_sock;
	SocketAddress m_addr;
};

CU_NS_END
