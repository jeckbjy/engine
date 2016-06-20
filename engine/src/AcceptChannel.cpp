#include "AcceptChannel.h"
#include "IOService.h"

CU_NS_BEGIN

AcceptChannel::AcceptChannel(Callback func, IOService* service)
: Channel(service)
, m_sock(INVALID_SOCKET)
, m_func(func)
, m_attached(false)
{
}

AcceptChannel::~AcceptChannel()
{
	detach();
	m_sock.close();
}

void AcceptChannel::perform(IOOperation* op)
{
	if (op->isKindOf<SyncOperation>())
	{
		SyncOperation* sop = (SyncOperation*)op;
		if (sop->isInput())
		{
			socket_t sock = m_sock.accept();
			error_t ec = getLastError();
			m_func(ec, sock);
		}
	}
	else if (op->isKindOf<AcceptOperation>())
	{
		AcceptOperation* aop = op->cast<AcceptOperation>();
		if (!aop)
			return;
		m_func(aop->code, aop->sock);
	}
}

void AcceptChannel::accept()
{
#ifdef CU_OS_WIN
	AcceptOperation* op = new AcceptOperation(this);
	op->sock = ::WSASocket(m_addr.family(), SOCK_STREAM, IPPROTO_IP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (op->sock != INVALID_SOCKET)
	{
		DWORD bytes, error;
		BOOL result = FAcceptEx(m_sock, op->sock, op->buff, 0, AcceptOperation::ADDR_LEN, AcceptOperation::ADDR_LEN, &bytes, &op->data);
		error = ::WSAGetLastError();
		if (result == FALSE && error != WSA_IO_PENDING)
			m_serivce->post(op, error);
	}
	else
	{
		m_serivce->post(op, WSAEBADF);
	}
#else
	// 每次获取一个
	socket_t sock = m_sock.accept();
	if (sock == INVALID_SOCKET && errno == EAGAIN)
	{// 监听等待回调
		m_serivce->modify(this, EV_CTL_MOD, EV_IN);
	}
	else
	{// 完成或出错
		completed(getLastError(), sock);
	}
#endif
}

void AcceptChannel::listen(const SocketAddress& addr, int backlog)
{
	if (m_sock != INVALID_SOCKET)
		return;
	m_addr = addr;
	m_sock.create(addr.family());
	m_sock.bind(addr, true);
	m_sock.listen(backlog);
	attach();
	accept();
}

void AcceptChannel::attach()
{
	if (m_attached || !m_serivce)
		return;
	if (m_serivce->attach((handle_t)m_sock.native(), this))
		m_attached = true;
}

void AcceptChannel::detach()
{
	if (m_attached)
	{
		m_serivce->detach((handle_t)m_sock.native(), this);
		m_attached = false;
	}
}

CU_NS_END