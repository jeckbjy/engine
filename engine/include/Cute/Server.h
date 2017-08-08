#pragma once
#include "Cute/Application.h"
#include "Cute/IOChannel.h"
#include "Cute/IOLoopGroup.h"
#include "Cute/ServerChannel.h"
#include "Cute/Session.h"
#include "Cute/EventQueue.h"
#include "Cute/Thread.h"
#include "Cute/ThreadPool.h"
#include "Cute/Mutex.h"
#include "Cute/Packet.h"
#include "Cute/SessionFilterChain.h"

CUTE_NS_BEGIN

/**
* 网络线程
* 单业务线程，如果业务线程挂掉需要重启
* 同步处理Session的Connect,Accept, Error消息
* 线程池,耗时操作需要单独线程中操作
* 不同的协议处理,Client:Packet格式，Admin:String格式
* 使用全局的HandlerService处理消息，但对于不同Session需要做权限验证
* 异常Hook，退出安全处理,DB数据安全写入
* TODO:现在是只有一个主逻辑线程，
* TODO:如何支持多线程，每个user分配到一个线程中执行并保证线程安全
* TODO:如何支持安模块划分执行事件，将阻塞任务延迟执行
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
	void	listen(const SocketAddress& addr, uint32 type = 0);
	void	connect(const SocketAddress& addr, uint32 type = 0);
	void	reconnect();

	void	post(EventBase* ev, uint32 delay = 0);
	void	schedule(Runnable* task);
	void	addPending(Session* sess, int mask);
    
protected:
    virtual void fireAccept(ServerChannel* listener, SocketChannel* channel);

protected:
	struct Pending
	{
		Session* sess;
		int		 events;
		Pending() :sess(0), events(0){}
	};

	typedef Map<uint32, ServerChannel*> AcceptorMap;
	typedef Map<uint32, Session*>		SessionMap;
	typedef Map<uint32, Pending>        PendingMap;
    typedef SessionFilterChain::Ptr     FilterChainPtr;

	bool		m_quit;
	uint32		m_maxid;			// 唯一ID
	uint32		m_frame;			// 帧频
	uint32		m_maxConnection;	// 最大连接数
	uint32		m_connectInterval;	// 重连间隔
	uint32		m_connectTime;		// 重连时间戳
	IOLoopGroup	m_loops;
	AcceptorMap	m_acceptors;
    SessionMap	m_sessions;
	SessionMap	m_connectors;
	PendingMap	m_pending;			// 将要处理的事件
	Mutex		m_mutex;
    Mutex		m_pendingMutex;
	EventQueue	m_events;
	Thread		m_logicThread;
	ThreadPool	m_pools;
    FilterChainPtr m_filterChain;
};

CUTE_NS_END
