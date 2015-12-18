#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"

CU_NS_BEGIN

class SocketAddress;
class CU_API SocketChannel : public Channel
{
public:
	SocketChannel(EventLoop* loop);
	~SocketChannel();

	void connect(const SocketAddress& addr);
	void write();
	void read();

	void completed(IOOperation* op);

	handle_t handle() const { return (handle_t)m_sock.native(); }

private:
	Socket m_sock;
	Buffer m_reader;
	Buffer m_writer;
};

CU_NS_END