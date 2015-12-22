#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"
#include "Mutex.h"

CU_NS_BEGIN

// �ص�
class IChannelHandler
{
public:
	virtual ~IChannelHandler(){}
	virtual void invoke(Channel* channel, int events) = 0;
};

class SocketAddress;
class CU_API SocketChannel : public Channel
{
public:
	SocketChannel(EventLoop* loop = NULL);
	~SocketChannel();

	void connect(const SocketAddress& addr);
	void send(const Buffer& buf);
	void recv();

	void perform(IOOperation* op);
	void completed(uint8_t type);

	handle_t handle() const { return (handle_t)m_sock.native(); }

private:
	void write();
	void read();

private:
	enum
	{
		F_CONNECTING = 0x01,
		F_WRITING = 0x02,
	};

	uchar  m_flags;
	Socket m_sock;
	Mutex  m_mutex;
	Buffer m_reader;
	Buffer m_writer;
};

CU_NS_END