#pragma once
#ifndef _WIN32

#include "Cute/Foundation.h"

#if defined(CUTE_OS_FAMILY_LINUX)
#  include <sys/epoll.h>
#  include <sys/eventfd.h>
#elif defined(CUTE_OS_FAMILY_BSD)
#  include <sys/event.h>
#endif

CUTE_NS_BEGIN

/*
1、listen fd，有新连接请求，触发EPOLLIN。
2、对端发送普通数据，触发EPOLLIN。
3、带外数据，只触发EPOLLPRI。
4、对端正常关闭（程序里close()，shell下kill或ctr+c），触发EPOLLIN和EPOLLRDHUP，但是不触发EPOLLERR和EPOLLHUP。
关于这点，以前一直以为会触发EPOLLERR或者EPOLLHUP。
再man epoll_ctl看下后两个事件的说明，这两个应该是本端（server端）出错才触发的。
5、对端异常断开连接（只测了拔网线），没触发任何事件。
*/

#if defined(CUTE_OS_FAMILY_LINUX)
#	define event_t				epoll_event
#	define ev_get_udata(ev)		ev.data.ptr
#	define ev_is_input(ev)		(ev.events & EPOLLIN) != 0
#	define ev_is_output(ev)		(ev.events & EPOLLOUT)!= 0
//#define ev_is_pri(ev)			(ev.events & EPOLLPRI)!= 0

#elif defined(CUTE_OS_FAMILY_BSD)

#	define event_t				struct kevent
#	define ev_get_udata(ev)		ev.udata
#	define ev_is_input(ev)		ev.filter == EVFILT_READ
#	define ev_is_output(ev)		ev.filter == EVFILT_WRITE
//#define ev_is_pri(ev)			ev.filter == 
#endif

// 操作码定义
#if defined(CUTE_OS_FAMILY_LINUX)
#	define EV_CTL_ADD		EPOLL_CTL_ADD
#	define EV_CTL_DEL		EPOLL_CTL_DEL
#	define EV_CTL_MOD		EPOLL_CTL_MOD

#	define EV_IN			EPOLLIN
#	define EV_OUT			EPOLLOUT
#	define EV_PTI			EPOLLPRI
#	define EV_ET			EPOLLET

#elif defined(CUTE_OS_FAMILY_BSD)

#	define EV_CTL_ADD		EV_ADD
#	define EV_CTL_DEL		EV_DELETE
#	define EV_CTL_MOD		EV_ADD

#	define EV_IN			0x001
#	define EV_OUT			0x002
#	define EV_PTI			0x004
#	define EV_ET			0x008
#endif

class CUTE_CORE_API IOPoll
{
public:
	IOPoll();
	~IOPoll();

	int wait(event_t* events, int counts, int msec);
	int modify(int fd, int op, int events, void* udata);

	void wakeup();
	bool reset();
	bool check(void* udata);

private:
	int m_fd;
	int m_reader;
	int m_writer;
};

CUTE_NS_END

#endif
