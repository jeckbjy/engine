#pragma once
#ifndef _WIN32

#include "Cute/Foundation.h"

#ifdef CUTE_OS_LINUX
#include <sys/epoll.h>
#include <sys/eventfd.h>
#endif

CUTE_NS_BEGIN

/*
1��listen fd�������������󣬴���EPOLLIN��
2���Զ˷�����ͨ���ݣ�����EPOLLIN��
3���������ݣ�ֻ����EPOLLPRI��
4���Զ������رգ�������close()��shell��kill��ctr+c��������EPOLLIN��EPOLLRDHUP�����ǲ�����EPOLLERR��EPOLLHUP��
������㣬��ǰһֱ��Ϊ�ᴥ��EPOLLERR����EPOLLHUP��
��man epoll_ctl���º������¼���˵����������Ӧ���Ǳ��ˣ�server�ˣ�����Ŵ����ġ�
5���Զ��쳣�Ͽ����ӣ�ֻ���˰����ߣ���û�����κ��¼���
*/

#if defined(CUTE_OS_FAMILY_LINUX)
#	define event_t				epoll_event
#	define ev_get_udata(ev)		ev.data.ptr
#	define ev_is_input(ev)		(ev.events & EPOLLIN) != 0
#	define ev_is_output(ev)		(ev.events & EPOLLOUT)!= 0
//#define ev_is_pri(ev)			(ev.events & EPOLLPRI)!= 0

#elif defined(CUTE_OS_FAMILY_BSD)

#	define event_t				kevent
#	define ev_get_udata(ev)		ev.udata
#	define ev_is_input(ev)		ev.filter == EVFILT_READ
#	define ev_is_output(ev)		ev.filter == EVFILT_WRITE
//#define ev_is_pri(ev)			ev.filter == 
#endif

// �����붨��
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
