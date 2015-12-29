#include "SocketChannel.h"
#include "IOService.h"

CU_NS_BEGIN

SocketChannel::SocketChannel(IOService* loop)
: Channel(loop)
, m_sock(INVALID_SOCKET)
, m_connecting(0)
{

}

SocketChannel::~SocketChannel()
{
}

void SocketChannel::reconnect()
{
#ifdef CU_OS_WIN
	if (FConnectEx == NULL)
		throw std::runtime_error("cannot find connectex");
	SocketOperation* op = new SocketOperation(this, SocketOperation::OP_CONNECT);
	DWORD result, err;
	result = FConnectEx(m_sock, m_peer.address(), m_peer.length(), 0, 0, 0, &op->data);
	err = ::WSAGetLastError();
	if (!result && err != WSA_IO_PENDING)
		m_serivce->post(op, err);
#else
	if (m_sock.connect(m_peer) == 0)
	{// 链接成功
		completed(SocketOperation::OP_CONNECT);
	}
	else
	{
		m_code = last_error();
		bool blocking = (m_code == EINPROGRESS || m_code == EWOULDBLOCK);
		if (blocking)
		{// blocking:wait for triger
			m_connecting = TRUE;
			m_serivce->modify(this, EV_CTL_MOD, EV_IN | EV_OUT);
		}
		else
		{// error:notify 
			notify(EV_ERROR);
		}
	}
#endif
}

void SocketChannel::connect(const SocketAddress& addr)
{
	if (m_sock == INVALID_SOCKET)
	{
		m_sock.create(addr.family());
		// iocp need bind
		m_sock.bind(SocketAddress(), true);
	}
	attach();
	m_peer = addr;
	// do connect
	reconnect();
}

void SocketChannel::send(const Buffer & buf)
{
	LockGuard<Mutex> guard(m_mutex);
	m_writer.append(buf);
	write();
}

void SocketChannel::send(const char* str)
{
	Buffer buf;
	buf.write(str, strlen(str));
	send(buf);
}

void SocketChannel::recv()
{
	m_serivce->recv(this);
}

void SocketChannel::write()
{
	while (!m_writer.eof())
	{
		char* buf = m_writer.chunk_data();
		int len = m_writer.chunk_size();
		int ret = m_sock.send(buf, len);
		if (ret <= 0)
		{// has error
			m_code = last_error();
			if (m_code == ERR_IN_PROGRESS)
				m_serivce->send(this);
			else
				notify(EV_ERROR);
			break;
		}

		m_writer.seek(len, SEEK_CUR);
		if (ret < len)
		{
			m_serivce->send(this);
			break;
		}
	}
}

void SocketChannel::read()
{
	// 从尾部添加数据
	m_reader.seek(0, SEEK_END);
	for (;;)
	{
		int len = m_sock.available();
		if (len <= 0)
			return;
		// 先预分配内存
		m_reader.expand((size_t)len);
		int bytes = 0;
		// 读取填充每段buf
		for (;;)
		{
			char*  buf = m_reader.chunk_data();
			size_t len = m_reader.chunk_size();
			int ret = m_sock.recv(buf, len);
			if (ret <= 0)
			{// error
				break;
			}
			// 向前移动
			bytes += ret;
			m_reader.seek(ret, SEEK_CUR);
		}
		assert(bytes == len);
	}
}

void SocketChannel::perform(IOOperation* op)
{
	if (!op->success())
	{
		m_code = op->code;
		// close log notify
		notify(EV_ERROR);
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
				m_code = ERR_CONNECTION_RESET;
				notify(EV_ERROR);
			}
			else
			{
				completed(SocketOperation::OP_READ);
			}
		}

		if (sop->isOutput())
		{// for write
			if (m_connecting)
				completed(SocketOperation::OP_CONNECT);
			// also try read
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
	LockGuard<Mutex> guard(m_mutex);
	switch (type)
	{
	case SocketOperation::OP_CONNECT:
	{
		m_connecting = FALSE;
		notify(EV_CONNECT);
	}
	break;
	case SocketOperation::OP_WRITE:
	{
		write();
		notify(EV_WRITE);
	}
	break;
	case SocketOperation::OP_READ:
	{// 同步读取直到结束
		read();
		notify(EV_READ);
		// 继续监听读取
		if (m_sock)
			m_serivce->recv(this);
	}
	break;
	}
}

CU_NS_END
