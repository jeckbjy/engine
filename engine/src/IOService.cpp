#include "IOService.h"
#include "Channel.h"

#ifndef CU_OS_WIN
#include "Poller.h"
#endif

CU_NS_BEGIN

#ifdef CU_OS_WIN
#define KEY_WAKEUP		1
#define KEY_COMPLETION	2
#endif

void IOService::WorkThread(void* param)
{
	IOService* service = (IOService*)param;
	service->run();
}

IOService::IOService()
: m_stopped(TRUE)
, m_threads(0)
, m_blocks(0)
{
#ifdef CU_OS_WIN
	m_handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 10000000);
	if (m_handle == INVALID_HANDLE_VALUE)
		throw std::runtime_error("create iocp error.");
#endif
}

IOService::~IOService()
{

}

void IOService::run()
{
	++m_threads;
	m_stopped = 0;
	while (!m_stopped)
	{
		++m_blocks;
		run_once(-1);
		--m_blocks;
	}
	--m_threads;
	if (m_blocks != 0)
		wakeup();
}

void IOService::stop()
{
	if (m_stopped == 0)
	{
		m_stopped = 1;
		if (m_threads != 0)
			wakeup();
	}
}

void IOService::wakeup()
{
#if defined(CU_OS_WIN)
	if (::PostQueuedCompletionStatus(m_handle, 0, KEY_WAKEUP, 0) == 0)
		throw std::runtime_error("wakeup:PostQueuedCompletionStatus error.");
#else
	((Poller*)m_handle)->wakeup();
#endif
}

#ifdef CU_OS_WIN
void IOService::run_once(int msec)
{
	DWORD bytes;
	DWORD_PTR completion_key = 0;
	LPOVERLAPPED overlapped = NULL;
	BOOL result = ::GetQueuedCompletionStatus(m_handle, &bytes, &completion_key, &overlapped, (DWORD)msec);
	// 唤醒处理
	if (completion_key == KEY_WAKEUP || overlapped == NULL)
		return;
	DWORD error = ::GetLastError();
	IOOperation::IOData* io_data = (IOOperation::IOData*)overlapped;
	IOOperation* op = io_data->op;
	Channel* channel = op->channel;

	if (completion_key == KEY_COMPLETION)
	{
		error = io_data->Offset;
		bytes = io_data->OffsetHigh;
	}
	op->code = error;
	if (op->isKindOf<AcceptOperation>())
	{
		AcceptOperation* aop = (AcceptOperation*)op;
		if (error == ERROR_SUCCESS)
		{// 更新
			SOCKET update_ctx = (SOCKET)channel->handle();
			::setsockopt(aop->sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&update_ctx, sizeof(SOCKET));
		}
		else
		{
			::closesocket(aop->sock);
			aop->sock = INVALID_SOCKET;
		}
		channel->perform(op);
	}
	else if (op->isKindOf<SocketOperation>())
	{
		SocketOperation* sop = (SocketOperation*)op;
		channel->perform(op);
	}
	delete op;
}
#else
void IOService::run_once(int msec)
{
	static const int kEventMax = 128;
	event_t events[kEventMax];
	int nums = m_handle.wait(events, kEventMax, msec);
	for (int i = 0; i < nums; ++i)
	{
		event_t& ev = events[i];
		void* udata = ev_get_udata(ev);
		if (m_handle.check(udata))
			continue;
		Channel* channel = (Channel*)udata;
		uint8_t flags = 0;
		if (ev_is_input(ev))
			flags |= SyncOperation::OP_INPUT;
		if (ev_is_output(ev))
			flags |= SyncOperation::OP_OUTPUT;
		SyncOperation op(channel, flags);
		op.code = errno;
		channel->perform(&op);
	}
}
#endif

bool IOService::attach(Channel* channel)
{
#ifdef CU_OS_WIN
	return ::CreateIoCompletionPort((HANDLE)channel->handle(), m_handle, 0, 0) == 0;
#else
	return m_handle.ctrl(channel->handle(), EV_CTL_ADD, EV_IN | EV_OUT, channel) == 0;
#endif
}

void IOService::detach(Channel* channel)
{
#ifndef CU_OS_WIN
	m_handle.ctrl(channel->handle(), EV_CTL_DEL, EV_IN | EV_OUT, channel);
#endif
}

void IOService::send(Channel* channel)
{
#ifdef CU_OS_WIN
	SocketOperation* op = new SocketOperation(channel, SocketOperation::OP_WRITE);
	WSABUF buf = { 0, 0 };
	DWORD bytes;
	int   result = ::WSASend((SOCKET)channel->handle(), &buf, 1, &bytes, 0, &op->data, 0);
	DWORD ec = ::WSAGetLastError();
	if (result != ERROR_SUCCESS)
	{
		if (ec == ERROR_PORT_UNREACHABLE)
			ec = WSAECONNREFUSED;
		if (ec != WSA_IO_PENDING)
			post(op, ec, bytes);
	}
#else
	modify(channel, EV_CTL_MOD, EV_OUT);
#endif
}

void IOService::recv(Channel* channel)
{
#ifdef CU_OS_WIN
	SocketOperation* op = new SocketOperation(channel, SocketOperation::OP_READ);
	WSABUF buf = { 0, 0 };
	DWORD bytes = 0, flag = 0;
	int result = ::WSARecv((SOCKET)channel->handle(), &buf, 1, &bytes, &flag, &op->data, 0);
	if (result != ERROR_SUCCESS)
	{
		DWORD ec = ::WSAGetLastError();
		if (ec != WSA_IO_PENDING)
		{
			if (ec == ERROR_NETNAME_DELETED)
				ec = WSAECONNRESET;
			else if (ec == ERROR_PORT_UNREACHABLE)
				ec = WSAECONNREFUSED;
			post(op, ec, bytes);
		}
	}
#else
	modify(channel, EV_CTL_MOD, EV_IN);
#endif
}

#ifdef CU_OS_WIN
void IOService::post(IOOperation* op)
{
	if (::PostQueuedCompletionStatus(m_handle, 0, KEY_COMPLETION, &op->data) == 0)
		throw std::runtime_error("post fail.");
}

void IOService::post(IOOperation* op, DWORD ec, DWORD bytes /* = 0 */)
{
	op->data.Offset = ec;
	op->data.OffsetHigh = bytes;
	post(op);
}
#else
void IOService::modify(Channel* channel, int op, int events)
{
	m_handle.ctrl(channel->handle(), op, events, channel);
}
#endif

CU_NS_END