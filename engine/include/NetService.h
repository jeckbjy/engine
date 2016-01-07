#pragma once
#include "API.h"
#include "NetEvent.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOServicePool.h"

CU_NS_BEGIN

enum NetType
{
	NET_NONE,
	NET_ACCEPTOR,
	NET_CONNECTOR,
	NET_SESSION,
};

struct NetInfo
{
	uint	mode;
	uint	type;	// 自定义类型
	String	host;
	NetInfo() :mode(NET_NONE), type(0){}
	NetInfo(uint mode, const String& host, uint type = 0) :mode(mode), type(type), host(host){}
};
typedef std::vector<NetInfo> NetInfoVec;

struct CU_API NetConfig
{
	uint	services;
	uint	workers;
	uint	reconnect_time;	// 重新尝试连接时间，sec
	NetInfoVec infos;
	NetConfig() :services(1), workers(1),reconnect_time(10){}
	void add_host(int mode, const String& host, uint type = 0);
};

// 网络线程处理消息分发
// 主线程处理业务逻辑
class CU_API NetService
{
public:
	NetService();
	~NetService();

	void load();
	void run();
	void stop();
	void post(NetEvent* se);
	void kick(uint sessid);

	virtual bool init();
	virtual void quit();
	virtual void loop();
	virtual void update();

	virtual bool onEvent(NetEvent* ev);
	virtual void onError(Session* sess, error_t ec);
	virtual void onAccept(Acceptor* acceptor, socket_t sock);
	virtual void onConnect(Session* sess){}

	virtual IProtocol* getProtocol(int type);

protected:
	typedef HashMap<uint, Session*>		SessionMap;
	// 根据不同类型注册acceptor
	typedef HashMap<uint, Acceptor*>	AcceptorMap;
	typedef HashMap<uint, Session*>		ConnectorMap;
	typedef std::list<Session*>			SessionList;
	typedef std::queue<NetEvent*>		EventQueue;
	bool			m_quit;
	uint			m_frame;		// 帧频
	uint			m_maxID;		// 当前最大ID
	IOServicePool	m_services;
	AcceptorMap		m_acceptors;
	ConnectorMap	m_connectors;
	SessionMap		m_sessions;		// 所有接入连接
	SessionList		m_closing;		// 需要关闭的socket
	EventQueue		m_events;		// 待处理的消息
	Mutex			m_mutex;		// 事件互斥锁
	NetConfig		m_config;		// 配置信息
	uint64			m_reconnecting;	// 重新尝试连接时间，0：表示无需连接
	// timestamp
	uint64			m_tstart;		// 起始时间
	uint64			m_timestamp;	// 当前时间戳
	uint64			m_delta;		// 流逝时间
	double			m_now;			// 以秒为单位时间戳
};

extern NetService* gNetService;

CU_NS_END
