//! Server
#include "Cute/Server.h"
#include "Cute/TCPServerChannel.h"
#include "Cute/TCPSocketChannel.h"

CUTE_NS_BEGIN

static void ServerLogicThread(void* param)
{
	Server* server = (Server*)param;
	server->loop();
}

Server& Server::get()
{
	return (Server&)Application::instance();
}

Server::Server()
	: m_quit(false)
	, m_maxid(0)
	, m_frame(33)
	, m_maxConnection(0)
	, m_connectInterval(10)
	, m_connectTime(0)
{

}

Server::~Server()
{
	quit();
}

bool Server::setup()
{
	if (!Application::setup())
		return false;

	// �����߼��߳�
	m_logicThread.start(&ServerLogicThread, this);
	return true;
}

void Server::quit()
{

}

void Server::tick()
{
	// �������,��������
}

void Server::process()
{
	// ������Ϣ
	m_events.process();
	
	// �������,��������
	reconnect();

	// ���������Ӻ͹ر�
	m_mutex.lock();
	Session* pending;
	// �����������socket
	for (SessionMap::iterator itor = m_pending.begin(); itor != m_pending.end(); ++itor)
	{
		pending = itor->second;
		if (pending->isClosing())
		{// ɾ��
			m_sessions.erase(pending->getID());
			pending->close();
			delete pending;
		}
		else
		{// ���
			m_sessions[pending->getID()] = pending;
		}
	}

	m_mutex.unlock();
}

void Server::loop()
{
	m_quit = 0;
	while (!m_quit)
	{
		// �����¼�
		process();
		// �ȴ�
		tick();
		Thread::sleep(m_frame);
	}
}

void Server::kick(Session* sess)
{
	if (sess == NULL || sess->isClosing())
		return;

	Mutex::ScopedLock lock(m_mutex);

	sess->shutdown();
	sess->setClosing();
	m_pending[sess->getID()] = sess;
}

Session* Server::find(uint32 id)
{
	SessionMap::iterator itor = m_sessions.find(id);
	if (itor != m_sessions.end())
		return itor->second;

	return NULL;
}

void Server::listen(uint16 port, uint32 type)
{
	SocketAddress addr(port);
	ServerChannel* channel = new TCPServerChannel(m_loops.main());
	channel->setListener(this);
	channel->listen(addr);
	channel->setID(newID());
	channel->setType(type);
	m_acceptors[channel->getID()] = channel;
}

void Server::connect(const SocketAddress& addr, uint32 type)
{
	SocketChannel* channel = new TCPSocketChannel(m_loops.main());
	Session* sess = new Session(channel, newID(), type);
	m_connectors[sess->getID()] = sess;
	sess->connect(addr);
}

void Server::reconnect()
{
	if (m_connectTime == 0)
		return;

	uint32 now = (uint32)time(NULL);
	if (now < m_connectTime)
		return;

	m_connectTime = 0;
	for (SessionMap::iterator itor = m_connectors.begin(); itor != m_connectors.end(); ++itor)
	{
		Session* session = itor->second;
		session->reconnect();
	}
}

uint32 Server::newID()
{
	return m_maxid++;
}

void Server::post(LogicEvent* ev, uint32 delay)
{
	ev->setDelay(delay);
	m_events.push(ev);
}

void Server::schedule(Runnable* task)
{
	m_pools.start(*task);
}

void Server::fireAccept(ServerChannel* listener, SocketChannel* channel)
{
	// ���µ�����
	channel->setLoop(m_loops.next());
	Session* sess = new Session(channel, newID(), listener->getType());

	Mutex::ScopedLock lock(m_mutex);
	m_pending[sess->getID()] = sess;
}

CUTE_NS_END
