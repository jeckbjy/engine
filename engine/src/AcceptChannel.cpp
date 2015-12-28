#include "AcceptChannel.h"
#include "IOService.h"

CU_NS_BEGIN

AcceptChannel::AcceptChannel(IOService* service)
: Channel(service)
, m_sock(INVALID_SOCKET)
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
		completed(last_error(), sock);
	}
#endif
}

void AcceptChannel::listen(const SocketAddress& addr)
{
	if (m_sock != INVALID_SOCKET)
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
	if (op->isKindOf<SyncOperation>())
	{
		SyncOperation* sop = (SyncOperation*)op;
		if (sop->isInput())
		{
			socket_t sock = m_sock.accept();
			error_t ec = last_error();
			completed(sock);
		}
	}
	else if (op->isKindOf<AcceptOperation>())
	{
		AcceptOperation* aop = op->cast<AcceptOperation>();
		if (!aop)
			return;
		completed(aop->sock);
	}
}

void AcceptChannel::completed(socket_t sock)
{
	// default
	Socket::close_socket(sock);
}

CU_NS_END