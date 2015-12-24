#include "NetService.h"
#include "Thread.h"
#include "Proto.h"

CU_NS_BEGIN

NetService* gNetService = NULL;

NetService::NetService()
: m_quit(true)
, m_frame(33)
{
	gNetService = this;
}

NetService::~NetService()
{
	if (gNetService == this)
		gNetService = NULL;
}

void NetService::run()
{
	if (!init())
		return;
	loop();
	quit();
}

void NetService::stop()
{
	m_quit = true;
}

void NetService::post(NetEvent* se)
{
	m_mutex.lock();
	m_events.push(se);
	m_mutex.unlock();
}

void NetService::loop()
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
				NetEvent* se = events.front();
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

bool NetService::init()
{
	return true;
}

void NetService::quit()
{
}

void NetService::update()
{
	Thread::sleep(m_frame);
}

void NetService::onAccept(Acceptor* acceptor, socket_t sock)
{

}

bool NetService::onPacket(Session* sess, IPacket* msg)
{
	delete msg;
	return true;
}

CU_NS_END