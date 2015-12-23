#include "SocketChannel.h"
#include "IOService.h"

CU_NS_BEGIN

SocketChannel::SocketChannel(IOService* loop)
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
		m_serivce->post(op, err);
#else
	if (m_sock.connect(addr) == 0) {
		// ���ӳɹ�
		completed(SocketOperation::OP_CONNECT);
	}

	bool blocking = (errno == EINPROGRESS || errno == EWOULDBLOCK);
	if (blocking){
		// blocking:wait for triger
		m_flags |= F_CONNECTING;
		m_serivce->modify(this, EV_CTL_MOD, EV_IN | EV_OUT);
	}else{
		// error:notify 
	}
#endif
}

void SocketChannel::send(const Buffer & buf)
{
	LockGuard<Mutex> guard(m_mutex);
	m_writer.append(buf);
	write();
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
		if (ret > 0) 
		{
			m_writer.seek(len, SEEK_CUR);
			if (ret < len)
				m_serivce->send(this);
		}
		else if (ret == -1)
		{
			error_t code = last_error();
			if (code == ERR_IN_PROGRESS)
			{
				m_serivce->send(this);
			}
			else
			{// notify error

			}
		}
	}
}

void SocketChannel::read()
{
	// ��β���������
	m_reader.seek(0, SEEK_END);
	for (;;)
	{
		int len = m_sock.available();
		if (len <= 0)
			return;
		// ��Ԥ�����ڴ�
		m_reader.expand((size_t)len);
		int bytes = 0;
		// ��ȡ���ÿ��buf
		for (;;)
		{
			char*  buf = m_reader.chunk_data();
			size_t len = m_reader.chunk_size();
			int ret = m_sock.recv(buf, len);
			if (ret <= 0)
			{// error
				break;
			}
			// ��ǰ�ƶ�
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
	LockGuard<Mutex> guard(m_mutex);
	switch (type)
	{
	case SocketOperation::OP_CONNECT:
	{
		m_flags &= ~F_CONNECTING;
		// ֪ͨ
	}
	break;
	case SocketOperation::OP_WRITE:
	{
		m_flags &= ~F_WRITING;
		write();
	}
	break;
	case SocketOperation::OP_READ:
	{// ͬ����ȡֱ������
		read();
		// ����������ȡ
		if (m_sock)
			m_serivce->recv(this);
	}
	break;
	}
	notify(type);
}

CU_NS_END
