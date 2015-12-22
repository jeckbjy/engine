#pragma once
#include "API.h"
#ifndef _WIN32
#include "Poller.h"
#endif

CU_NS_BEGIN

class IOOperation;
class Channel;
// iocp or poller
class CU_API EventLoop
{
public:
	EventLoop();
	~EventLoop();

	void run();
	void stop();
	void wakeup();
	void run_once(int msec);

	bool attach(Channel* channel);
	void detach(Channel* channel);
	// 处理
	void send(Channel* channel);
	void recv(Channel* channel);

#ifdef CU_OS_WIN
	void post(IOOperation* op);
	void post(IOOperation* op, DWORD ec, DWORD bytes = 0);
#else
	void modify(Channel* channel, int op, int events);
#endif

private:
#ifdef CU_OS_WIN
	typedef HANDLE service;
#else
	typedef Poller service;
#endif
	service	 m_handle;
	Atomic8	 m_stopped;
	Atomic32 m_threads;	// 运行中的线程
	Atomic32 m_blocks;	// 阻塞中的
};

CU_NS_END