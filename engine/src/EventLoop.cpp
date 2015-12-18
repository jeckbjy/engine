#include "EventLoop.h"
#include "Channel.h"

#ifndef CU_OS_WIN
#include "Poller.h"
#endif

CU_NS_BEGIN

#ifdef CU_OS_WIN
#define KEY_WAKEUP		1
#define KEY_COMPLETION	2
#endif

EventLoop::EventLoop()
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::run()
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

void EventLoop::stop()
{
	if (m_stopped == 0)
	{
		m_stopped = 1;
		if (m_threads != 0)
			wakeup();
	}
}

void EventLoop::wakeup()
{
#if defined(CU_OS_WIN)
	if (::PostQueuedCompletionStatus((HANDLE)m_handle, 0, KEY_WAKEUP, 0) == 0)
		throw std::runtime_error("wakeup:PostQueuedCompletionStatus error.");
#else
	((Poller*)m_handle)->wakeup();
#endif
}

#ifdef CU_OS_WIN
void EventLoop::run_once(int msec)
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
		channel->completed(op);
	}
	else if (op->isKindOf<SocketOperation>())
	{
		SocketOperation* sop = (SocketOperation*)op;
		channel->completed(op);
	}
	delete op;
}
#else
void EventLoop::run_once(int msec)
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
		channel->completed(&op);
	}
}
#endif

bool EventLoop::attach(Channel* channel)
{
#ifdef CU_OS_WIN
	return ::CreateIoCompletionPort((HANDLE)channel->handle(), m_handle, 0, 0) == 0;
#else
	return m_handle.ctrl(channel->handle(), EV_CTL_ADD, EV_IN | EV_OUT, channel) == 0;
#endif
}

void EventLoop::detach(Channel* channel)
{
#ifndef CU_OS_WIN
	m_handle.ctrl(channel->handle(), EV_CTL_DEL, EV_IN | EV_OUT, channel);
#endif
}

#ifdef CU_OS_WIN
void EventLoop::post(IOOperation* op)
{
	if (::PostQueuedCompletionStatus(m_handle, 0, KEY_COMPLETION, &op->data) == 0)
		throw std::runtime_error("post fail.");
}

void EventLoop::post(IOOperation* op, DWORD ec, DWORD bytes /* = 0 */)
{
	op->data.Offset = ec;
	op->data.OffsetHigh = bytes;
	post(op);
}
#else
void EventLoop::modify(Channel* channel, int op, int events)
{
	m_handle.ctrl(channel->handle(), op, events, channel);
}
#endif

CU_NS_END