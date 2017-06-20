#pragma once
#include "Cute/Foundation.h"
#include "Cute/Runnable.h"
#include "Cute/Thread.h"
#include "Cute/Delegate.h"

CUTE_NS_BEGIN

enum NotifyMask
{
	NOTIFY_ACCESS		= 0x0001,
	NOTIFY_MODIFY		= 0x0002,
	NOTIFY_ATTRIB		= 0x0004,
	NOTIFY_OPEN			= 0x0008,
	NOTIFY_CREATE		= 0x0010,
	NOTIFY_DELETE		= 0x0020,
	NOTIFY_DELETE_SELF	= 0x0080,
	NOTIFY_MOVE_SELF	= 0x0100,
	NOTIFY_MOVED_FROM	= 0x0200,
	NOTIFY_MOVED_TO		= 0x0400,
	NOTIFY_CLOSE_WRITE	= 0x0800,
	NOTIFY_CLOSE_READ	= 0x1000,

	NOTIFY_MOVE			= NOTIFY_MOVED_FROM | NOTIFY_MOVED_TO,
	NOTIFY_CLOSE		= NOTIFY_CLOSE_WRITE | NOTIFY_CLOSE_READ,
	NOTIFY_ALL			= 0xFFFF,
};

struct NotifyEvent
{
	String	watcher;
	String	filename;
	uint32	mask;
	bool	isDir;
};

class DirectoryWatcherStrategy;
typedef Delegate<void(NotifyEvent*)> DirectoryWatcherDelegate;
// 目录文件监控,
// ReadDirectoryChanges
// http://www.cnblogs.com/guomeiran/p/4056876.html
// http://blog.csdn.net/zhoushuaiyin/article/details/42639053
// http://blog.csdn.net/woshinia/article/details/8162983
// https://www.codeproject.com/kb/files/directorychangewatcher.aspx
// http://www.ibm.com/developerworks/cn/aix/library/1105_huangrg_kqueue/
// http://www.cnblogs.com/fengyv/archive/2012/07/30/2614783.html			// all selector
class CUTE_CORE_API DirectoryWatcher
{
public:
	static DirectoryWatcher& get();

	DirectoryWatcher();
	~DirectoryWatcher();

	void start();
	void stop();

	void watch(const String& path, uint32 events, DirectoryWatcherDelegate cb);
	void remove(const String& path);

private:
	typedef DirectoryWatcherStrategy Strategy;
	Thread		m_thread;
	Strategy*	m_strategy;
	//IOLoop		m_loop;
	// 对于window和bsd每个文件对应一个监听
};

CUTE_NS_END