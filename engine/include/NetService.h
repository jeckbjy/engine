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
	uint	type;	// �Զ�������
	String	host;
	NetInfo() :mode(NET_NONE), type(0){}
	NetInfo(uint mode, const String& host, uint type = 0) :mode(mode), type(type), host(host){}
};
typedef std::vector<NetInfo> NetInfoVec;

struct CU_API NetConfig
{
	uint	services;
	uint	workers;
	uint	reconnect_time;	// ���³�������ʱ�䣬sec
	NetInfoVec infos;
	NetConfig() :services(1), workers(1),reconnect_time(10){}
	void add_host(int mode, const String& host, uint type = 0);
};

// �����̴߳�����Ϣ�ַ�
// ���̴߳���ҵ���߼�
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
	// ���ݲ�ͬ����ע��acceptor
	typedef HashMap<uint, Acceptor*>	AcceptorMap;
	typedef HashMap<uint, Session*>		ConnectorMap;
	typedef std::list<Session*>			SessionList;
	typedef std::queue<NetEvent*>		EventQueue;
	bool			m_quit;
	uint			m_frame;		// ֡Ƶ
	uint			m_maxID;		// ��ǰ���ID
	IOServicePool	m_services;
	AcceptorMap		m_acceptors;
	ConnectorMap	m_connectors;
	SessionMap		m_sessions;		// ���н�������
	SessionList		m_closing;		// ��Ҫ�رյ�socket
	EventQueue		m_events;		// ���������Ϣ
	Mutex			m_mutex;		// �¼�������
	NetConfig		m_config;		// ������Ϣ
	uint64			m_reconnecting;	// ���³�������ʱ�䣬0����ʾ��������
	// timestamp
	uint64			m_tstart;		// ��ʼʱ��
	uint64			m_timestamp;	// ��ǰʱ���
	uint64			m_delta;		// ����ʱ��
	double			m_now;			// ����Ϊ��λʱ���
};

extern NetService* gNetService;

CU_NS_END
