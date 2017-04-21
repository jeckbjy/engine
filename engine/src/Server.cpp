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

	// 启动逻辑线程
	m_logicThread.start(&ServerLogicThread, this);
	return true;
}

void Server::quit()
{

}

void Server::tick()
{
	// 心跳检测,断线重连
}

void Server::process()
{
	// 处理消息
	m_events.process();
	
	// 心跳检测,断线重连
	reconnect();

	// 处理pending事件
	m_pendingMutex.lock();
	for (PendingMap::iterator itor = m_pending.begin(); itor != m_pending.end(); ++itor)
	{
		Pending& pending = itor->second;
		Session* sess = pending.sess;
		int events = pending.events;

		if ((events & (PE_REMOVE || PE_ERROR)) != 0)
			sess->close();

		if ((events & PE_ERROR) != 0)
		{
			onError(sess);
		}
		else
		{
			if ((events & PE_ACCEPT) != 0)
				onAccept(sess);

			if ((events & PE_CONNECT) != 0)
				onConnect(sess);

			if ((events & PE_SEND) != 0)
				onSend(sess);
		}

		m_sessions.erase(sess->getID());
	}

	m_pendingMutex.unlock();
}

void Server::loop()
{
	m_quit = 0;
	while (!m_quit)
	{
		// 处理事件
		process();
		// 等待
		tick();
		Thread::sleep(m_frame);
	}
}

void Server::kick(Session* sess)
{
	if (sess == NULL || sess->isClosing())
		return;

	sess->shutdown();
	sess->setClosing();
	addPending(sess, PE_REMOVE);
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
	Session* sess = new Session(&m_protocal, channel, newID(), type);
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
	// 有新的连接
	Session* sess = new Session(&m_protocal, channel, newID(), listener->getType());

	addPending(sess, PE_ACCEPT);
}

void Server::addPending(Session* sess, int mask)
{
	Mutex::ScopedLock lock(m_pendingMutex);
	Pending& pending = m_pending[sess->getID()];
	if (pending.sess == 0)
	{
		pending.sess = sess;
		pending.events = mask;
	}
	else if (pending.sess == sess)
	{
		pending.events |= mask;
	}
	else
	{
		assert(false);
	}
}

void Server::onAccept(Session* sess)
{
	CUTE_UNUSED(sess);
}

void Server::onConnect(Session* sess)
{
	sess->getChannel()->setLoop(m_loops.next());
}

void Server::onSend(Session* sess)
{
	CUTE_UNUSED(sess);
}

void Server::onError(Session* sess)
{
	CUTE_UNUSED(sess);
}

CUTE_NS_END
