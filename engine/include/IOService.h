#pragma once
#include "API.h"
#include "Atomic.h"
#ifndef _WIN32
#include "Poller.h"
#endif

CU_NS_BEGIN

class IOOperation;
class Channel;
// iocp or poller
class CU_API IOService
{
public:
	static void WorkThread(void* param);

	IOService();
	~IOService();

	bool isStopped() const { return m_stopped == TRUE; }
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
	service	m_handle;
	Atomic	m_stopped;
	Atomic	m_threads;	// 运行中的线程
	Atomic	m_blocks;	// 阻塞中的
};

CU_NS_END