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
	if (m_sock.connect(addr) == 0) {
		// 链接成功
		completed(SocketOperation::OP_CONNECT);
	}

	bool blocking = (errno == EINPROGRESS || errno == EWOULDBLOCK);
	if (blocking){
		// blocking:wait for triger
		m_flags |= F_CONNECTING;
		m_loop->modify(this, EV_CTL_MOD, EV_IN | EV_OUT);
	}else{
		// error:notify 
		//op.code = errno;
		//perform(&op);
	}
#endif
}

void SocketChannel::write(const Buffer & buf)
{
	m_writer.write(buf);
	send();
}

void SocketChannel::send()
{
	while (!m_writer.eof())
	{
		int len = m_writer.chunk_size();
		int ret = m_sock.send(m_writer.chunk_data(), len);
		if(ret == 0)
			break;
		if (ret > 0) {
			m_writer.advance(len);
			if (ret < len)
				m_loop->send(this);
		}
		else if (ret == -1)
		{
			error_t code = last_error();
			if (code == ERR_IN_PROGRESS)
			{
				m_loop->send(this);
			}
			else
			{// notify error

			}
		}
	}
}

void SocketChannel::read()
{
	int len = m_sock.available();
	if (len <= 0)
		return;
	m_reader.reserve((size_t)len);
	for (;;)
	{
		char*  buf = m_reader.chunk_data();
		size_t len = m_reader.chunk_size();
		int ret = m_sock.recv(buf, len);
	}
	// 监听读
	//m_loop->read();
}

void SocketChannel::perform(IOOperation* op)
{
	if (!op->success())
	{
		// close log notify
		return;
	}
	if (op->isKindOf<SyncOperation>())
	{// for *nix
		SyncOperation* sop = (SyncOperation*)(op);
		if (sop->isInput())
		{// for read
			int len = m_sock.available();
			if (len == 0)
			{// error:socket closed
			}
			completed(SocketOperation::OP_READ);
		}

		if (sop->isOutput())
		{// for write
			if (m_flags & F_CONNECTING)
				completed(SocketOperation::OP_CONNECT);
			completed(SocketOperation::OP_WRITE);
		}
	}
	else if(op->isKindOf<SocketOperation>())
	{// for iocp
		completed(((SocketOperation*)op)->type);
	}
}

void SocketChannel::completed(uint8_t type)
{
	switch (type)
	{
	case SocketOperation::OP_CONNECT:
	{
		m_flags &= ~F_CONNECTING;
		// 通知
	}
	break;
	case SocketOperation::OP_WRITE:
	{
		m_flags &= ~F_WRITING;
		send();
	}
	break;
	case SocketOperation::OP_READ:
	{// 同步读取直到结束
		read();
	}
	break;
	}
}

CU_NS_END
