//! Network/Sockets
#include "Cute/TCPServerChannel.h"
#include "Cute/TCPSocketChannel.h"
#include "Cute/IORequest.h"
#include "Cute/SocketDefs.h"
#include "Cute/IOLoop.h"

CUTE_NS_BEGIN

TCPServerChannel::TCPServerChannel(IOLoop* loop)
	: ServerChannel(loop)
	, m_backlog(0)
	, m_family(AF_INET)
	, m_req(0)
{
	setFlag(MASK_SOCKET, true);
}

TCPServerChannel::~TCPServerChannel()
{
	delete m_req;
}

void TCPServerChannel::listen(const SocketAddress& addr)
{
	if (m_handle != INVALID_HANDLE_VALUE)
		throw NetException("TCPServerChannel isActive!");

	if (m_listener == NULL)
		throw NetException("TCPServerChannel::bind:listener is null!");

	// AF_INET AF_INET6 AF_UNSPEC
	socket_t sock = sock_create(AF_UNSPEC, SOCK_STREAM, IPPROTO_IP);
	m_handle = (handle_t)sock;
	m_req = new AcceptReq(this);

	sock_bind(sock, addr.address(), addr.length());
	sock_listen(sock, m_backlog);
	attach();
	accept();
	modify(EVENT_IN);
}

void TCPServerChannel::accept()
{
#ifdef CUTE_OS_FAMILY_WINDOWS
	socket_t sock = ::WSASocket(m_family, SOCK_STREAM, IPPROTO_IP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		throw NetException("create socket error!");

	m_req->setSocket(sock);
	DWORD bytes;
	BOOL  result = FUN_ACCEPTEX((socket_t)m_handle, sock, m_req->getBuffer(), 0, m_req->getLength(), m_req->getLength(), &bytes, m_req->getData());
	DWORD error = ::WSAGetLastError();
	if (result == FALSE && error != WSA_IO_PENDING)
	{
		m_loop->post(m_req, error);
	}
#endif
}

void TCPServerChannel::selection(int events)
{
	if (events & EVENT_IN)
	{
		for (;;)
		{
			socket_t sock = sock_accept((socket_t)m_handle, NULL, NULL);
			if (sock == INVALID_SOCKET)
				break;

			sock_setblocking(sock, false);
			m_req->setSocket(sock);
			completed(m_req);
		}
	}
}

void TCPServerChannel::completed(IORequest* req)
{
	assert(req->isKindOf<AcceptReq>());
	AcceptReq* areq = req->cast<AcceptReq>();
	if (areq->isSuccess())
	{
		socket_t sock = areq->getSocket();
		TCPSocketChannel* new_socket = new TCPSocketChannel(NULL);
		new_socket->open(sock);
		m_listener->fireAccept(this, new_socket);
	}

	// iocp重新投递
	accept();
}

CUTE_NS_END
