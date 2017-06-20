//! FileSystem
#include "Cute/DirectoryWatcher.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOWS)
class DirectoryWatcherStrategy
{

};
#elif defined(CUTE_OS_FAMILY_LINUX)
class DirectoryWatcherStrategy
{
public:
	DirectoryWatcherStrategy()
		: m_epoll(0)
		, m_inotify(0)
	{
		m_inotify = inotify_init();
		if (m_inotify <= 0)
			throw IOException("cannot initialize inotify", errno);

		m_epoll = epoll_create(EPOLL_CLOEXEC);
		if (m_epoll <= 0)
			throw IOException("cannot initialize epoll", errno);
	}

	~DirectoryWatcherStrategy()
	{
		if (m_inotify != 0)
			close(m_inotify);

		if (m_epoll != 0)
			close(m_epoll);
	}

	void watch(const String& path, uint32 events, DirectoryWatcherDelegate cb)
	{
		int mask = 0;
		int wd = inotify_add_watch(m_inotify, path.c_str(), mask);
		if (wd == -1)
			throw FileException("inotify_add_watch", errno);

	}

	void run()
	{
		event_t events[128];
		while (true)
		{
			int counts = epoll_wait(m_inotify, events, 128, 0);

		}
	}


private:
	int m_epoll;
	int m_inotify;
};
#elif defined(CUTE_OS_FAMILY_BSD)
class DirectoryWatcherStrategy
{
    
};
#else
#error "not support!!"
#endif
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
DirectoryWatcher::DirectoryWatcher()
	: m_strategy(NULL)
{
}

DirectoryWatcher::~DirectoryWatcher()
{
	delete m_strategy;
}

void DirectoryWatcher::start()
{
	if (m_strategy == NULL)
	{
		m_strategy = new DirectoryWatcherStrategy();
	}

	if (!m_thread.isRunning())
	{
		//m_thread.start(*m_strategy)
	}
}

void DirectoryWatcher::stop()
{
	//m_strategy->stop();
	m_thread.join();
}

CUTE_NS_END
