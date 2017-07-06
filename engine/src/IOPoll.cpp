//! IO
#include "Cute/IOPoll.h"
#ifndef _WIN32
CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_LINUX)
// beg linux
#	if !defined(EPOLL_CLOEXEC)
#	define EPOLL_CLOEXEC 20000
#	endif
// check event fd
#	if defined(EFD_CLOEXEC) &&  defined(EFD_NONBLOCK)
#	define EFD_ARGS EFD_CLOEXEC | EFD_NONBLOCK
#	else
#	define EFD_ARGS 0
#	endif
// end linux
#elif defined(CUTE_OS_FAMILY_BSD)
// beg bsd
#ifdef __NetBSD__
#define KQ_UTYPE intptr_t
#else
#define KQ_UTYPE void*
#endif
// end bsd
#endif

IOPoll::IOPoll()
{
#if defined(CUTE_OS_FAMILY_LINUX)
	m_fd = epoll_create(EPOLL_CLOEXEC);
	m_reader = m_writer = ::eventfd(0, EFD_ARGS);
#elif defined(CUTE_OS_FAMILY_BSD)
	m_fd = kqueue();
	m_reader = m_writer = -1;
#endif
	if (m_fd == -1)
		throw std::runtime_error("create reactor poll fd error");
	// pipe方式创建
	if (m_reader == -1)
	{
		int fds[2];
		if (pipe(fds) != 0)
			throw std::runtime_error("poll create pipe error");
		m_reader = fds[0];
		m_writer = fds[1];
	}
	// 设置
	::fcntl(m_reader, F_SETFL, O_NONBLOCK);
	::fcntl(m_reader, F_SETFD, FD_CLOEXEC);
	::fcntl(m_writer, F_SETFL, O_NONBLOCK);
	::fcntl(m_writer, F_SETFD, FD_CLOEXEC);
	// 添加监听interupt
	modify(m_reader, EV_CTL_ADD, EV_IN, this);
}

IOPoll::~IOPoll()
{
	if (m_reader != -1)
	{
		// 注销
		modify(m_reader, EV_CTL_DEL, EV_IN, NULL);
		::close(m_reader);
		if (m_reader != m_writer)
			::close(m_writer);
		m_reader = m_writer = -1;
	}
	if (m_fd != -1)
	{
		::close(m_fd);
		m_fd = -1;
	}
}

int IOPoll::wait(event_t* events, int counts, int msec)
{
	int nums = 0;
	for (;;)
	{
#if defined(CUTE_OS_FAMILY_LINUX)
		nums = ::epoll_wait(m_fd, events, counts, msec);
#elif defined(CUTE_OS_FAMILY_BSD)
		timespec timeout = { msec, 0 };
		nums = ::kevent(m_fd, 0, 0, events, counts, &timeout);
#endif
		if (nums >= 0)
			break;
		if (nums == -1)
		{// 是否发生错误
			if (errno != EINTR && errno != EAGAIN)
				throw std::runtime_error("poll wait error.");
		}
	}
	return nums;
}

int IOPoll::modify(int fd, int op, int events, void* udata)
{
#ifdef CUTE_OS_FAMILY_LINUX
	epoll_event ev = { 0, { 0 } };
	ev.events = events | EPOLLERR | EPOLLHUP;
	ev.data.ptr = udata;
	return epoll_ctl(m_fd, op, fd, &ev);
#elif defined(CUTE_OS_FAMILY_BSD)
	struct kevent ev[2];
	int num = 0;
	op |= EV_CLEAR;
	if ((events & EV_IN) != 0)
	{// 添加
		EV_SET(&ev[num], fd, EVFILT_READ, op, 0, 0, (KQ_UTYPE)udata);
		++num;
	}
	if ((events & EV_OUT) != 0)
	{
		EV_SET(&ev[num], fd, EVFILT_WRITE, op, 0, 0, (KQ_UTYPE)udata);
		++num;
	}
	return ::kevent(m_fd, ev, num, 0, 0, 0);
#endif
}

void IOPoll::wakeup()
{
#if defined(CUTE_OS_FAMILY_LINUX)
	epoll_event ev = { 0, { 0 } };
	ev.events = EPOLLIN | EPOLLERR | EPOLLET;
	ev.data.ptr = this;
	epoll_ctl(m_fd, EPOLL_CTL_MOD, m_reader, &ev);
#elif defined(CUTE_OS_FAMILY_BSD)
	uint64_t counter(1UL);
	::write(m_writer, &counter, sizeof(uint64_t));
#endif
}

bool IOPoll::reset()
{
	if (m_reader == m_writer)
	{
		for (;;)
		{
			uint64_t counter(0);
			errno = 0;
			long bytes = (long)::read(m_reader, &counter, sizeof(uint64_t));
			if (bytes < 0 && errno == EINTR)
				continue;
			return bytes > 0;
		}
	}
	else
	{
		// Clear all data from the pipe.
		for (;;)
		{
			char data[32];
			long bytes = (long)::read(m_reader, data, sizeof(data));
			if (bytes < 0 && errno == EINTR)
				continue;
			bool interrupted = bytes > 0;
			while (bytes == sizeof(data))
				bytes = ::read(m_reader, data, sizeof(data));
			return interrupted;
		}
	}
}

bool IOPoll::check(void* udata)
{
	if (udata != this)
		return false;
	reset();
	return true;
}

CUTE_NS_END
#endif
