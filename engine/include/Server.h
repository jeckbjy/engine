#pragma once
#include "API.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOService.h"
#include "ServerEvent.h"

CU_NS_BEGIN

// 网络线程处理消息分发
// 主线程处理业务逻辑,NetService??
class CU_API Server
{
public:
	Server();
	~Server();

	void run();
	void stop();
	void post(ServerEvent* se);

	virtual bool init();
	virtual void quit();
	virtual void loop();
	virtual void update();

	virtual void onError(Session* sess){}
	virtual void onConnect(Session* sess){}
	virtual void onAccept(Acceptor* acceptor, socket_t sock){}

protected:
	typedef HashMap<uint, Session*>		SessionMap;
	// 根据不同类型注册acceptor
	typedef HashMap<uint, Acceptor*>	AcceptorMap;
	typedef std::queue<ServerEvent*>	EventQueue;
	bool			m_quit;
	uint			m_frame;		// 帧频
	IOServicePool	m_services;
	AcceptorMap		m_acceptors;
	Session*		m_connector;	// 向上连接
	SessionMap		m_sessions;		// 所有连接
	EventQueue		m_events;		// 待处理的消息
	Mutex			m_mutex;		// 事件互斥锁
};

extern Server* gServer;

CU_NS_END
