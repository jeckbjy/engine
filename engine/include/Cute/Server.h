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
#include "Cute/Protocal.h"

CUTE_NS_BEGIN

/**
* �����߳�
* ��ҵ���̣߳����ҵ���̹߳ҵ���Ҫ����
* ͬ������Session��Connect,Accept, Error��Ϣ
* �̳߳�,��ʱ������Ҫ�����߳��в���
* ��ͬ��Э�鴦��,Client:Packet��ʽ��Admin:String��ʽ
* ʹ��ȫ�ֵ�HandlerService������Ϣ�������ڲ�ͬSession��Ҫ��Ȩ����֤
* �쳣Hook���˳���ȫ����,DB���ݰ�ȫд��
*/
class CUTE_CORE_API Server : public Application, public ServerListener
{
public:
	enum PendingEvent
	{
		PE_ERROR	= 0x01,
		PE_ACCEPT	= 0x02,
		PE_CONNECT	= 0x04,
		PE_SEND		= 0x08,
		PE_REMOVE	= 0x10,
	};

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
	void	addPending(Session* sess, int mask);

protected:
	virtual void fireAccept(ServerChannel* listener, SocketChannel* channel);
	virtual void onAccept(Session* sess);
	virtual void onConnect(Session* sess);
	virtual void onSend(Session* sess);
	virtual void onError(Session* sess);

protected:
	struct Pending
	{
		Session* sess;
		int		 events;
		Pending() :sess(0), events(0){}
	};

	typedef std::map<uint32, ServerChannel*>	AcceptMap;
	typedef std::map<uint32, Session*>			SessionMap;
	typedef std::map<uint32, Pending>			PendingMap;

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
	PendingMap	m_pending;			// ��Ҫ������¼�
	Mutex		m_pendingMutex;
	AcceptMap	m_acceptors;
	LogicQueue	m_events;
	Thread		m_logicThread;
	ThreadPool	m_pools;
	PacketProtocal m_protocal;
};

CUTE_NS_END
