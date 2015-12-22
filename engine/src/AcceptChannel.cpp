#include "AcceptChannel.h"
#include "EventLoop.h"

CU_NS_BEGIN

AcceptChannel::AcceptChannel(EventLoop* loop)
:Channel(loop)
{

}

AcceptChannel::~AcceptChannel()
{
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
			m_loop->post(op, error);
	}
	else
	{
		m_loop->post(op, WSAEBADF);
	}
#else
	// 每次获取一个
	socket_t sock = m_sock.accept();
	if (sock == INVALID_SOCKET && errno == EAGAIN)
	{// 监听等待回调
		m_loop->modify(this, EV_CTL_MOD, EV_IN);
	}
	else
	{// 完成或出错
		completed(last_error(), sock);
	}
#endif
}

void AcceptChannel::listen(const SocketAddress& addr)
{
	if (m_sock)
		return;
	m_addr = addr;
	m_sock.create(addr.family());
	m_sock.bind(addr, true);
	m_sock.listen();
	attach();
	accept();
}

void AcceptChannel::perform(IOOperation* op)
{
	AcceptOperation* aop = op->cast<AcceptOperation>();
	if (!aop)
		return;
	completed(aop->code, aop->sock);
}

void AcceptChannel::completed(error_t ec, socket_t sock)
{
	if (ec != 0 || sock == INVALID_SOCKET)
	{// 说明出错了
	}
	// 创建
}

CU_NS_END