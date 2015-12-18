#include "SocketChannel.h"
#include "EventLoop.h"

CU_NS_BEGIN

SocketChannel::SocketChannel(EventLoop* loop)
:Channel(loop)
{

}

SocketChannel::~SocketChannel()
{
}

void SocketChannel::connect(const SocketAddress& addr)
{
	if (!m_sock)
	{
		m_sock.create(addr.family());
		// iocp need bind
		m_sock.bind(SocketAddress(), true);
	}
	attach();
	// do connect
#ifdef CU_OS_WIN
	if (FConnectEx == NULL)
		throw std::runtime_error("cannot find connectex");
	SocketOperation* op = new SocketOperation(this, SocketOperation::OP_CONNECT);
	DWORD result, err;
	result = FConnectEx(m_sock, addr.address(), addr.length(), 0, 0, 0, &op->data);
	err = ::WSAGetLastError();
	if (!result && err != WSA_IO_PENDING)
		m_loop->post(op, err);
#else
	SocketOperation op(this, SocketOperation::OP_CONNECT);
	if (m_sock.connect(addr) == 0)
		completed(&op);

	bool blocking = errno == EINPROGRESS || errno == EWOULDBLOCK;
	if (blocking){
		m_loop->modify(this, EV_CTL_MOD, EV_IN | EV_OUT);
	}else{
		op.code = errno;
		completed(&op);
	}
#endif
}

void SocketChannel::write()
{

}

void SocketChannel::read()
{

}

void SocketChannel::completed(IOOperation* op)
{
	SocketOperation* sop = op->cast<SocketOperation>();
	if (sop == NULL)
		return;
	if (!op->success()){
		// close log notify
		return;
	}

	switch (sop->type)
	{
	case SocketOperation::OP_CONNECT:
	{

	}
	break;
	case SocketOperation::OP_WRITE:
	{

	}
	break;
	case SocketOperation::OP_READ:
	{// 同步读取直到结束

	}
	break;
	}
}

CU_NS_END
