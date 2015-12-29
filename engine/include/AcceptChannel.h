#pragma once
#include "Channel.h"
#include "Socket.h"
#include "Delegate.h"

CU_NS_BEGIN

class CU_API AcceptChannel : public Channel
{
public:
	typedef Delegate<void(error_t, socket_t)> Callback;

	AcceptChannel(Callback func, IOService* service);
	~AcceptChannel();

	void accept();
	void listen(const SocketAddress& addr, int backlog = 64);
	virtual void perform(IOOperation* op);

	void close() { m_sock.close(); }
	void reset() { m_func.reset(); }
	void setCallbackOwner(void* owner) { m_func.setObject(owner); }
	handle_t handle() const { return (handle_t)m_sock.native(); }

protected:
	Socket m_sock;
	Callback m_func;
	SocketAddress m_addr;
};

CU_NS_END
