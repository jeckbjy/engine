#include "ProxyChannel.h"
#include "IOService.h"

CU_NS_BEGIN

ProxyChannel::ProxyChannel(IOService* loop, socket_t input, const SocketAddress& peerAddr)
	: Channel(loop)
	, m_input(input)
	, m_state(S_DISCONNECT)
	, m_addr(peerAddr)
{
	assert(!m_input.invalid());

	//do connect peer
	m_output.create(m_addr.family());
	m_output.setBlocking(false);
#ifdef _WIN32
	// for iocp
	m_output.bind(SocketAddress(), true);
#endif
	m_serivce->attach((handle_t)m_output.native(), this);
	reconnect();

	// register input
	m_input.setBlocking(false);
	m_serivce->attach((handle_t)m_input.native(), this);
	// ��ʱ��������
	//m_serivce->recv(m_input.native(), this);
}

ProxyChannel::~ProxyChannel()
{
	m_serivce->detach((handle_t)m_input.native(), this);
	m_serivce->detach((handle_t)m_output.native(), this);

	m_input.close();
	m_output.close();
}

void ProxyChannel::perform(IOOperation* op)
{
	if (!op->success())
	{
		return;
	}
	// ִ��
	if (op->isKindOf<SyncOperation>())
	{// unix
		SyncOperation* sop = (SyncOperation*)(op);
		if (sop->isInput())
		{// for read
			int len = m_input.available();
			if (len == 0)
			{// error:socket closed
				m_code = ERR_CONNECTION_RESET;
				//notify(EV_ERROR);
			}
			else
			{
				completed(SocketOperation::OP_READ);
			}
		}

		if (sop->isOutput())
		{// for write
			if (m_state == S_CONNECTING)
				completed(SocketOperation::OP_CONNECT);
			// also try read
			completed(SocketOperation::OP_WRITE);
		}
	}
	else if (op->isKindOf<SocketOperation>())
	{
		completed(((SocketOperation*)op)->type);
	}
}

void ProxyChannel::completed(uint8_t type)
{
	LockGuard<Mutex> guard(m_mutex);
	switch (type)
	{
	case SocketOperation::OP_CONNECT:
	{
		m_state = S_ESTABLISH;
		// ��ʼ������ȡ
		if (!m_input.invalid())
			m_serivce->recv(m_input.native(), this);
	}
	break;
	case SocketOperation::OP_WRITE:
	{
		write();
		//notify(EV_WRITE);
	}
	break;
	case SocketOperation::OP_READ:
	{// ͬ����ȡֱ������
		read();
		//notify(EV_READ);
		// ����������ȡ
		if (!m_input.invalid())
			m_serivce->recv(m_input.native(), this);
	}
	break;
	}
}

void ProxyChannel::reconnect()
{
	if (m_state != S_DISCONNECT)
		return;
	m_state = S_CONNECTING;
	m_serivce->connect(m_addr, m_output.native(), this);
}

void ProxyChannel::read()
{
	// ��ȡ��ֱ�ӷ��͸���һ��
	char buff[1024];
	for (;;)
	{
		int ret = m_input.recv(buff, 1024);
		if (ret < 0)
			break;
		// д�뵽��һ��,������д����ʱ������??
		// send();
		//int wret = m_output.send(buff, ret);
		//if (wret < 0)
		//{
		//	// catch??
		//}
	}
}

void ProxyChannel::write()
{

}

CU_NS_END
