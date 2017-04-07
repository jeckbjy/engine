//! IO
#include "Cute/IOLoop.h"
#include "Cute/IOChannel.h"
#include "Cute/IORequest.h"
#include "Cute/Exception.h"
#include "Cute/IOPoll.h"

#ifdef _WIN32
#	define KEY_WAKEUP		1
#	define KEY_COMPLETION	2
#endif

CUTE_NS_BEGIN

IOLoop::IOLoop()
	: m_stopped(TRUE)
	, m_threads(0)
	, m_blocks(0)
	, m_handle(NULL)
{
#if defined(_WIN32)
	m_handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 10000000);
	if (m_handle == INVALID_HANDLE_VALUE)
		throw SystemException("create iocp error.");
#else
	m_handle = new IOPoll();
#endif
}

IOLoop::~IOLoop()
{
#if defined(_WIN32)
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
#else
	delete m_handle;
#endif
}

void IOLoop::run()
{
	++m_threads;
	m_stopped = 0;
	while (!m_stopped)
	{
		++m_blocks;
		once(-1);
		--m_blocks;
	}
	--m_threads;
	if (m_blocks != 0)
		wakeup();
}

void IOLoop::stop()
{
	if (m_stopped == 0)
	{
		m_stopped = 1;
		if (m_threads != 0)
			wakeup();
	}
}

void IOLoop::wakeup()
{
#if defined(_WIN32)
	if (::PostQueuedCompletionStatus(m_handle, 0, KEY_WAKEUP, 0) == 0)
		throw std::runtime_error("wakeup:PostQueuedCompletionStatus error.");
#else
	((IOPoll*)m_handle)->wakeup();
#endif
}

void IOLoop::once(long msec)
{
#if defined(_WIN32)
	DWORD bytes;
	DWORD_PTR completion_key = 0;
	LPOVERLAPPED overlapped = NULL;
	SetLastError(0);
	BOOL result = ::GetQueuedCompletionStatus(m_handle, &bytes, &completion_key, &overlapped, (DWORD)msec);
	// »½ÐÑ´¦Àí
	if (completion_key == KEY_WAKEUP || overlapped == NULL)
		return;

	IORequest::IOData* io_data = (IORequest::IOData*)overlapped;
	IORequest* op = io_data->op;
	IOProxy* proxy = op->getProxy();

	DWORD error = ::GetLastError();
	if (completion_key == KEY_COMPLETION)
	{
		error = io_data->Offset;
		bytes = io_data->OffsetHigh;
	}

	op->setCode(error);
	op->perform();
	proxy->completed(op);
#else
	IOPoll* poll = (IOPoll*)m_handle;
	static const int kEventMax = 128;
	event_t events[kEventMax];
	int nums = poll->wait(events, kEventMax, msec);
	for (int i = 0; i < nums; ++i)
	{
		event_t& ev = events[i];
		void* udata = ev_get_udata(ev);
		if (poll->check(udata))
			continue;

		IOChannel* channel = (IOChannel*)udata;

		uint8_t flags = 0;
		if (ev_is_input(ev))
			flags |= IOChannel::EVENT_IN;
		if (ev_is_output(ev))
			flags |= IOChannel::EVENT_OUT;

		channel->selection(flags);
	}
#endif
}

bool IOLoop::attach(IOChannel* channel)
{
#if defined(_WIN32)
	return ::CreateIoCompletionPort(channel->handle(), m_handle, 0, 0) != INVALID_HANDLE_VALUE;
#else
	return m_handle->modify(channel->handle(), EV_CTL_ADD, EV_IN | EV_OUT, channel) == 0;
#endif
}

bool IOLoop::detach(IOChannel* channel)
{
#if defined(_WIN32)
	return true;
#else
	return m_handle->modify(channel->handle(), EV_CTL_DEL, EV_IN | EV_OUT, channel) == 0;
#endif
}

#if defined(_WIN32)
void IOLoop::post(IORequest* op)
{
	if (::PostQueuedCompletionStatus(m_handle, 0, KEY_COMPLETION, &op->m_data) == 0)
		throw std::runtime_error("post fail.");
}

void IOLoop::post(IORequest* op, DWORD ec, DWORD bytes /* = 0 */)
{
	op->m_data.Offset = ec;
	op->m_data.OffsetHigh = bytes;
	post(op);
}
#else
void IOLoop::modify(IOChannel* channel, int events)
{
	m_handle->modify(channel->handle(), EV_CTL_MOD, events, channel);
}
#endif

CUTE_NS_END