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
* 网络线程
* 单业务线程，如果业务线程挂掉需要重启
* 线程池,耗时操作需要单独线程中操作
* 不同的协议处理,Client:Packet格式，Admin:String格式
* 异常Hook，退出安全处理,DB数据安全写入
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
	uint32		m_maxid;			// 唯一ID
	uint32		m_frame;			// 帧频
	uint32		m_maxConnection;	// 最大连接数
	uint32		m_connectInterval;	// 重连间隔
	uint32		m_connectTime;		// 重连时间戳
	Mutex		m_mutex;
	IOLoopGroup	m_loops;
	SessionMap	m_sessions;
	SessionMap	m_connectors;
	SessionMap	m_pending;			// 新连接或者需要关闭的
	AcceptMap	m_acceptors;
	LogicQueue	m_events;
	Thread		m_logicThread;
	ThreadPool	m_pools;
};

CUTE_NS_END
