#pragma once
#include "Cute/Foundation.h"
#include "Cute/Runnable.h"
#include "Cute/Atomic.h"

CUTE_NS_BEGIN

/*
Asynchronous TCP and UDP sockets
Asynchronous DNS resolution
Asynchronous file and file system operations ??
IPC with socket sharing, using Unix domain sockets or named pipes (Windows)
*/
// One Loop One Thread
// CancelIo
class IOPoll;
class IOChannel;
class IORequest;
// https://my.oschina.net/u/2001537/blog/353035
// http://tinyclouds.org/iocp-links.html			// libuv
class CUTE_CORE_API IOLoop : public Runnable
{
public:
	IOLoop();
	~IOLoop();

	void run();
	void start();
	void stop();
	void wakeup();
	void once(long msec);

	bool attach(IOChannel* channel);
	bool detach(IOChannel* channel);

public:
#ifdef _WIN32
	void post(IORequest* op);
	void post(IORequest* op, DWORD ec, DWORD bytes = 0);
#else
	void modify(IOChannel* channel, int events);
#endif

private:
#ifdef _WIN32
	typedef handle_t	loop_t;
#else
	typedef IOPoll*		loop_t;
#endif
	loop_t	m_handle;
	Atomic	m_stopped;
	Atomic	m_threads;
	Atomic	m_blocks;
};

CUTE_NS_END
