//! IO
#include "Cute/IOChannel.h"
#include "Cute/IOLoop.h"
#include "Cute/SocketDefs.h"
#include "Cute/SocketAddress.h"

CUTE_NS_BEGIN

IOChannel::IOChannel(IOLoop* loop)
	: m_proxy(new IOProxy(this))
	, m_loop(loop)
	, m_handle(INVALID_HANDLE_VALUE)
{
#ifdef CUTE_OS_FAMILY_POSIX
	m_events = 0;
#endif
}

IOChannel::~IOChannel()
{
	m_proxy->reset();
}

void IOChannel::close()
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
#ifdef _WIN32
		if (isSocket())
			sock_close((socket_t)m_handle);
		else
			::CloseHandle(m_handle);
#else
		::close(m_handle);
#endif
		detach();
		m_handle = INVALID_HANDLE_VALUE;
		// 清空状态
		m_flag &= ~MASK_STATUS;
	}
}

void IOChannel::attach()
{
	if (!isRegistered() && m_loop != NULL)
	{
		if (m_loop->attach(this))
			setFlag(MASK_REGISTER, true);
	}
}

void IOChannel::detach()
{
	if (isRegistered() && m_loop != NULL)
	{
		m_loop->detach(this);
		setFlag(MASK_REGISTER, false);
	}
}

void IOChannel::bind(const SocketAddress& addr, bool reuse)
{
	assert(isSocket() && isOpen());

	if (isBound())
		return;

	socket_t sock = (socket_t)m_handle;
	if (reuse)
	{
		sock_setoption(sock, SOL_SOCKET, SO_REUSEADDR, true);
		sock_setreuseport(sock, true);
	}

	sock_bind(sock, addr.address(), addr.length());
	setFlag(MASK_BOUND, true);
}

handle_t IOChannel::handle() const
{
	return m_handle;
}

socket_t IOChannel::socket() const
{
	return (socket_t)m_handle;
}

IOLoop* IOChannel::getLoop()
{
	return m_loop;
}

void IOChannel::setLoop(IOLoop* loop)
{
	if (m_loop != NULL)
		throw InvalidArgumentException("loop is not NULL!");

	m_loop = loop;
	attach();
}

IOProxy* IOChannel::getProxy()
{
	return m_proxy;
}

void IOChannel::modify(int events)
{
#ifdef CUTE_OS_FAMILY_POSIX
	int pevents = m_events | events;
	if (pevents != m_events)
	{
		m_loop->modify(this, events);
	}
#endif
}

void IOChannel::setSocket(socket_t sock)
{
	m_handle = (handle_t)sock;
}

//////////////////////////////////////////////////////////////////////////
// IOChannelProxy
//////////////////////////////////////////////////////////////////////////
IOProxy::IOProxy(IOChannel* channel)
	: m_channel(channel)
{
}

void IOProxy::reset()
{
	Mutex::ScopedLock guard(m_mutex);
	if (m_channel)
		m_channel->close();
	m_channel = NULL;
}

void IOProxy::completed(IORequest* op)
{
	Mutex::ScopedLock guard(m_mutex);
	if (m_channel != NULL)
		m_channel->completed(op);
}

handle_t IOProxy::handle()
{
	Mutex::ScopedLock guard(m_mutex);
	if (m_channel != NULL)
		return m_channel->handle();
	return INVALID_HANDLE_VALUE;
}

CUTE_NS_END
