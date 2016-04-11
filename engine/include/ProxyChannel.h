#pragma once
#include "Channel.h"
#include "Buffer.h"
#include "Mutex.h"
#include "Socket.h"
#include "Delegate.h"

CU_NS_BEGIN

// ����Socket,��ȫֻ����Ϣת������������socket,����һ��buffer
// ��peer�Ͽ�����ʱ����Ҫ��������
// ��client��Ϣ����ʱ��Ҫ�Ͽ�����,��client�Ͽ�����ʱ��Ҫ�ж�����channel
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
	Socket	m_input;		// ��������Ϣ
	Socket	m_output;		// ��д����Ϣ
	Mutex	m_mutex;
	State	m_state;		// output����״̬
};

CU_NS_END