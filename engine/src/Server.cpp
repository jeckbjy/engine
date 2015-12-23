#include "Server.h"
#include "Thread.h"

CU_NS_BEGIN

Server* gServer = NULL;

Server::Server()
: m_quit(true)
, m_frame(33)
{
	gServer = this;
}

Server::~Server()
{
	if (gServer == this)
		gServer = NULL;
}

void Server::run()
{
	if (!init())
		return;
	loop();
	quit();
}

void Server::stop()
{
	m_quit = true;
}

void Server::post(ServerEvent* se)
{
	m_mutex.lock();
	m_events.push(se);
	m_mutex.unlock();
}

void Server::loop()
{
	m_quit = false;
	while (!m_quit)
	{
		EventQueue waits;
		// 处理消息
		m_mutex.lock();
		while (!m_events.empty())
		{
			EventQueue events;
			swap(events, m_events);
			m_mutex.unlock();
			while (!events.empty())
			{
				ServerEvent* se = events.front();
				events.pop();
				bool ok = se->process(this);
				if (!ok)
					waits.push(se);
				else
					delete se;
			}
			m_mutex.lock();
		}
		// wait for next frame to run
		swap(waits, m_events);
		m_mutex.unlock();
		// 
		update();
	}
}

bool Server::init()
{
	return true;
}

void Server::quit()
{
}

void Server::update()
{
	Thread::sleep(m_frame);
}

CU_NS_END