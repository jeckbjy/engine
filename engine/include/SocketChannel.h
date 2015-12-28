#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"
#include "Mutex.h"

CU_NS_BEGIN

class SocketAddress;
class CU_API SocketChannel : public Channel
{
public:
	SocketChannel(IOService* loop = NULL);
	virtual ~SocketChannel();

	virtual void notify(uint8_t type) = 0;

	void reconnect();
	void connect(const SocketAddress& addr);
	void send(const Buffer& buf);
	void send(const char* str);
	void recv();

	void perform(IOOperation* op);
	void completed(uint8_t type);

	handle_t handle() const { return (handle_t)m_sock.native(); }
	Buffer& getReader() { return m_reader; }
	Buffer& getWriter() { return m_writer; }

private:
	void write();
	void read();

protected:
	enum
	{
		EV_ERROR,
		EV_CONNECT,
		EV_READ,
		EV_WRITE,
	};
	uchar  m_connecting:1;
	Socket m_sock;
	Mutex  m_mutex;
	Buffer m_reader;
	Buffer m_writer;
	SocketAddress m_peer;
};

CU_NS_END