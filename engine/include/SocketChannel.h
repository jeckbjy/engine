#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"

CU_NS_BEGIN

class SocketAddress;
class CU_API SocketChannel : public Channel
{
public:
	SocketChannel(EventLoop* loop = NULL);
	~SocketChannel();

	void connect(const SocketAddress& addr);
	void write(const Buffer& buf);
	void read();

	void perform(IOOperation* op);
	void completed(uint8_t type);

	handle_t handle() const { return (handle_t)m_sock.native(); }

private:
	void send();
	void recv();

private:
	enum
	{
		F_CONNECTING = 0x01,
		F_WRITING = 0x02,
	};

	uchar  m_flags;
	Socket m_sock;
	Buffer m_reader;
	Buffer m_writer;
};

CU_NS_END