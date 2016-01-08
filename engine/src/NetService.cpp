#include "NetService.h"
#include "Thread.h"
#include "Log.h"
#include "Util.h"

CU_NS_BEGIN

NetService* gNetService = NULL;

#ifdef CU_OS_WIN
BOOL WINAPI onTermSignal(DWORD cevent)
{
	// 返回值含义：TRUE:阻止系统响应,FALSE:不阻止
	switch (cevent)
	{
	case CTRL_C_EVENT:			// ctrl + c
	case CTRL_BREAK_EVENT:		// ctrl + break
		return TRUE;
	case CTRL_CLOSE_EVENT:		// 点击close按钮
	case CTRL_LOGOFF_EVENT:		// 用户退出
	case CTRL_SHUTDOWN_EVENT:	// 系统关闭
		gNetService->quit();
		return FALSE;
	}
	return FALSE;
}
void hook_quit()
{
	SetConsoleCtrlHandler(onTermSignal, TRUE);
}
#else
// 监听退出
void onTermSignal(int signal_code)
{
	if (signal_code != SIGTERM)
		return;
	gNetService->quit();
}
void hook_quit()
{
	signal(SIGTERM, &onTermSignal);
}
#endif

void NetConfig::add_host(int mode, const String& host, uint type /* = 0 */)
{
	NetInfo info(mode, host, type);
	infos.push_back(info);
}

NetService::NetService()
: m_quit(true)
, m_frame(33)
, m_maxID(0)
, m_reconnecting(0)
{
	gNetService = this;
	m_tstart = m_timestamp = Util::time();
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
		LOG_TRACE("server run");
		hook_quit();
		loop();
		quit();
	}
	catch (std::exception& e)
	{
		LOG_ERROR("%s", e.what());
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

void NetService::kick(uint sessid)
{
	SessionMap::iterator itor = m_sessions.find(sessid);
	if (itor == m_sessions.end())
		return;
	Session* sess = itor->second;
	sess->shutdown();
	m_sessions.erase(itor);
	// delay free
	delete sess;
}

void NetService::loop()
{
	m_quit = false;
	while (!m_quit)
	{
		// 处理消息
		m_mutex.lock();
		// waiting for next frame process
		EventQueue waits;
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
		// update time
		uint64 last_time = m_timestamp;
		m_timestamp = Util::time();
		m_delta = m_timestamp - last_time;
		m_now = (double)(m_timestamp) / 1000;
		// execute one frame
		update();
	}
}

void NetService::update()
{
	// reconnect
	if (!m_reconnecting && m_timestamp - m_reconnecting > m_config.reconnect_time)
	{
		m_reconnecting = 0;
		for (size_t i = 0; i < m_connectors.size(); ++i)
		{
			m_connectors[i]->reconnect();
		}
	}
	// calc frame
	uint frame = m_delta >= m_frame ? 0 : m_frame - m_delta;
	Thread::sleep(frame);
}

bool NetService::init()
{
	gLog.addDefault();
	gLog.run();
	m_services.run(m_config.services, m_config.workers);
	NetInfoVec& infos = m_config.infos;
	for (NetInfoVec::iterator& itor = infos.begin(); itor != infos.end(); ++itor)
	{
		if (itor->mode == NET_ACCEPTOR)
		{
			assert(m_acceptors.find(itor->type) == m_acceptors.end());
			Acceptor* acceptor = new Acceptor(m_services.next(), itor->type);
			m_acceptors[itor->type] = acceptor;
			acceptor->listen(itor->host);
		}
		else if (itor->mode == NET_CONNECTOR)
		{
			assert(m_connectors.find(itor->type) == m_connectors.end());
			IProtocol* protocal = getProtocol(itor->type);
			Session* connector = new Session(m_maxID++, m_services.next(), protocal);
			connector->setType(itor->mode);
			m_connectors[itor->type] = connector;
			connector->connect(itor->host);
		}
	}
	return true;
}

void NetService::quit()
{
	if (m_quit)
		return;
	m_quit = true;
	LOG_WARN("quit net service");

	// close all socket
	for (SessionMap::iterator itor = m_sessions.begin(); itor != m_sessions.end(); ++itor)
	{
		Session* sess = itor->second;
		sess->close();
		delete sess;
	}
	m_sessions.clear();

	// close all connector
	for (size_t i = 0; i < m_connectors.size(); ++i)
	{
		Session* sess = m_connectors[i];
		sess->close();
		delete sess;
	}
	m_connectors.clear();

	// close all acceptor
	for (size_t i = 0; i < m_acceptors.size(); ++i)
	{
		Acceptor* acceptor = m_acceptors[i];
		acceptor->close();
		delete acceptor;
	}
	m_acceptors.clear();

	m_services.stop();
	gLog.stop();
}

bool NetService::onEvent(NetEvent* ev)
{
	return true;
}

void NetService::onError(Session* sess, error_t ec)
{
	if (sess->getType() == NET_CONNECTOR)
	{
		m_reconnecting = m_timestamp;
	}
	else
	{
		// kick session
		LOG_WARN("sess error:sessID = %d,error_code = %d", sess->getId(), ec);
		kick(sess->getId());
	}
}

void NetService::onAccept(Acceptor* acceptor, socket_t sock)
{
	Session* sess = new Session(m_maxID++, m_services.next(), getProtocol(0), sock);
	sess->setType(NET_SESSION);
	m_sessions[sess->getId()] = sess;
}

IProtocol* NetService::getProtocol(int type)
{
	return PacketProtocol::InstancePtr();
}

CU_NS_END