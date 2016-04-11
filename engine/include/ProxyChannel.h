#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Mutex.h"
#include "Socket.h"
#include "Delegate.h"

CU_NS_BEGIN

// 代理Socket,完全只做消息转发，持有两个socket,公用一个buffer
// 当peer断开连接时，需要断线重连
// 当client消息过大时需要断开连接,当client断开连接时需要中断整个channel
class CU_API ProxyChannel : public Channel
{
public:
	typedef Delegate<void(uint8_t)> Callback;

	ProxyChannel(IOService* loop, socket_t input, const SocketAddress& peerAddr);
	virtual ~ProxyChannel();

	void perform(IOOperation* op);
	void completed(uint8_t type);

	void reconnect();
	void read();
	void write();

protected:
	SocketAddress m_addr;	// output addr
	Socket	m_input;		// 仅读入消息
	Socket	m_output;		// 仅写出消息
	Mutex	m_mutex;
	State	m_state;		// output连接状态
};

CU_NS_END