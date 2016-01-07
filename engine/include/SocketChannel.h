#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Socket.h"
#include "Mutex.h"
#include "SocketAddress.h"
#include "Delegate.h"

CU_NS_BEGIN

class CU_API SocketChannel : public Channel
{
public:
	enum
	{
		EV_ERROR,
		EV_CONNECT,
		EV_READ,
		EV_WRITE,
	};
	typedef Delegate<void(uint8_t)> Callback;

	SocketChannel(Callback fun, IOService* loop, socket_t sock = INVALID_SOCKET);
	virtual ~SocketChannel();

	void reconnect();
	void connect(const SocketAddress& addr);
	void send(const Buffer& buf);
	void recv();

	void perform(IOOperation* op);
	void completed(uint8_t type);
	void notify(uint8_t type);

	void close() { m_sock.close(); }
	void shutdown(int how) { m_sock.shutdown(how); }

	handle_t handle() const { return (handle_t)m_sock.native(); }
	Buffer& getReader() { return m_reader; }
	Buffer& getWriter() { return m_writer; }

	void reset(){ m_fun.reset(); }
	void setCallbackOwner(void* owner) { m_fun.setObject(owner); }

private:
	void write();
	void read();

protected:
	enum State
	{
		S_DISCONNECT,
		S_CONNECTING,
		S_ESTABLISH,
	};
	Socket m_sock;
	State  m_state;
	Mutex  m_mutex;
	Buffer m_reader;
	Buffer m_writer;
	SocketAddress m_peer;
	Callback m_fun;
};

CU_NS_END