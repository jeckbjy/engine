#include "NetService.h"
#include "Thread.h"
#include "Proto.h"

CU_NS_BEGIN

NetService* gNetService = NULL;

void NetConfig::add_host(int mode, const String& host, uint type /* = 0 */)
{
	NetInfo info(mode, host, type);
	infos.push_back(info);
}

NetService::NetService()
: m_quit(true)
, m_frame(33)
, m_maxID(0)
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
	try
	{
		if (!init())
			return;
		loop();
		quit();
	}
	catch (std::exception&)
	{
	}
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
	m_services.run(m_config.services, m_config.workers);
	NetInfoVec& infos = m_config.infos;
	for (NetInfoVec::iterator& itor = infos.begin(); itor != infos.end(); ++itor)
	{
		if (itor->mode == NetInfo::T_ACCEPT)
		{
			assert(m_acceptors.find(itor->type) == m_acceptors.end());
			Acceptor* acceptor = new Acceptor(m_services.next(), itor->type);
			m_acceptors[itor->type] = acceptor;
			acceptor->listen(itor->host);
		}
		else if (itor->mode == NetInfo::T_CONNECT)
		{
			assert(m_connectors.find(itor->type) == m_connectors.end());
			IProtocol* protocal = getProtocol(itor->type);
			Session* connector = new Session(m_services.next(), m_maxID++, itor->type, protocal);
			m_connectors[itor->type] = connector;
			connector->connect(itor->host);
		}
	}
	return true;
}

void NetService::quit()
{
}

void NetService::update()
{
	Thread::sleep(m_frame);
}

bool NetService::onEvent(NetEvent* ev)
{
	return true;
}

IProtocol* NetService::getProtocol(int type)
{
	return PacketProtocol::InstancePtr();
}

CU_NS_END