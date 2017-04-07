#pragma once
#include "Cute/Application.h"
#include "Cute/IOChannel.h"
#include "Cute/IOLoopGroup.h"
#include "Cute/ServerChannel.h"
#include "Cute/Session.h"
#include "Cute/LogicQueue.h"
#include "Cute/Thread.h"
#include "Cute/ThreadPool.h"
#include "Cute/Mutex.h"
#include "Cute/Packet.h"

CUTE_NS_BEGIN

/**
* �����߳�
* ��ҵ���̣߳����ҵ���̹߳ҵ���Ҫ����
* �̳߳�,��ʱ������Ҫ�����߳��в���
* ��ͬ��Э�鴦��,Client:Packet��ʽ��Admin:String��ʽ
* �쳣Hook���˳���ȫ����,DB���ݰ�ȫд��
*/
class CUTE_CORE_API Server : public Application, public ServerListener, public SocketListener
{
public:
	static Server& get();

	Server();
	virtual ~Server();

	virtual bool setup();
	virtual void quit();
	virtual void tick();
	virtual void loop();
	virtual void process();

	virtual void kick(Session* sess);
	Session* find(uint32 id);

	uint32	newID();
	void	listen(uint16 port, uint32 type = 0);
	void	connect(const SocketAddress& addr, uint32 type = 0);
	void	reconnect();

	void	post(LogicEvent* ev, uint32 delay = 0);
	void	schedule(Runnable* task);

protected:
	virtual void fireAccept(ServerChannel* listener, SocketChannel* channel);

protected:
	typedef std::map<uint32, ServerChannel*>	AcceptMap;
	typedef std::map<uint32, Session*>			SessionMap;

	bool		m_quit;
	uint32		m_maxid;			// ΨһID
	uint32		m_frame;			// ֡Ƶ
	uint32		m_maxConnection;	// ���������
	uint32		m_connectInterval;	// �������
	uint32		m_connectTime;		// ����ʱ���
	Mutex		m_mutex;
	IOLoopGroup	m_loops;
	SessionMap	m_sessions;
	SessionMap	m_connectors;
	SessionMap	m_pending;			// �����ӻ�����Ҫ�رյ�
	AcceptMap	m_acceptors;
	LogicQueue	m_events;
	Thread		m_logicThread;
	ThreadPool	m_pools;
};

CUTE_NS_END
