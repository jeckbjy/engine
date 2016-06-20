#include "SocketChannel.h"
#include "IOService.h"

CU_NS_BEGIN

SocketChannel::SocketChannel(Callback fun, IOService* loop, socket_t sock)
: Channel(loop)
, m_sock(sock)
, m_state(sock == INVALID_SOCKET ? S_DISCONNECT : S_ESTABLISH)
, m_attached(false)
, m_fun(fun)
{
	if (!m_sock.invalid())
	{
		m_addr = m_sock.peerAddress();
		m_sock.setBlocking(false);
		attach();
		recv();
	}
}

SocketChannel::~SocketChannel()
{
	detach();
}

void SocketChannel::reconnect()
{
	if (m_state != S_DISCONNECT)
		return;
	m_state = S_CONNECTING;
	m_serivce->connect(m_addr, m_sock.native(), this);
}

void SocketChannel::connect(const SocketAddress& addr)
{
	detach();
	m_sock.close();

	m_addr = addr;
	m_sock.create(addr.family());
#ifdef _WIN32
	// because iocp need bind
	m_sock.bind(SocketAddress(), true);
#endif

	attach();
	// do connect
	reconnect();
}

void SocketChannel::send(const Buffer & buf)
{
	LockGuard<Mutex> guard(m_mutex);
	m_writer.append(buf);
	write();
}

void SocketChannel::recv()
{
	m_serivce->recv(m_sock.native(), this);
}

void SocketChannel::write()
{
	m_writer.seek(0, SEEK_SET);
	// todo:cannot seek
	while (!m_writer.eof())
	{
		char* buf = m_writer.chunk_data();
		int len = (int)m_writer.chunk_size();
		int ret = m_sock.send(buf, len);
		if (ret <= 0)
		{// has error
			m_code = getLastError();
			if (m_code == ERR_IN_PROGRESS)
				m_serivce->send(m_sock.native(), this);
			else
				notify(EV_ERROR);
			break;
		}

		m_writer.seek(len, SEEK_CUR);
		if (ret < len)
		{
			m_serivce->send(m_sock.native(), this);
			break;
		}
	}
	m_writer.discard();
}

void SocketChannel::read()
{
	// 从尾部添加数据
	m_reader.seek(0, SEEK_END);
	// available()无效暂时无法MSG_PEEK
	char* buf;
	uint len;
	for (;;)
	{
		m_reader.get_free(buf, len);
		int ret = m_sock.recv(buf, len);
		if (ret < 0)
			break;
		m_reader.expand((size_t)ret);
		m_reader.seek(ret, SEEK_CUR);
	}
}

void SocketChannel::attach()
{
	if (m_sock.invalid())
		return;

	if (m_attached || !m_serivce)
		return;

	if (m_serivce->attach((handle_t)m_sock.native(), this))
		m_attached = true;
}

void SocketChannel::detach()
{
	if (m_attached)
	{
		m_serivce->detach((handle_t)m_sock.native(), this);
		m_attached = false;
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
			if (m_state == S_CONNECTING)
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
		m_state = S_ESTABLISH;
		notify(EV_CONNECT);
		if (m_sock != INVALID_SOCKET)
			m_serivce->recv(m_sock.native(), this);
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
		if (m_sock != INVALID_SOCKET)
			m_serivce->recv(m_sock.native(), this);
	}
	break;
	}
}

void SocketChannel::notify(uint8_t type)
{
	m_fun(type);
	// 修改连接状态
	if (type == EV_ERROR)
		m_state = S_DISCONNECT;
}

CU_NS_END
