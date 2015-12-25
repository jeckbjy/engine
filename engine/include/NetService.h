#pragma once
#include "API.h"
#include "NetEvent.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOServicePool.h"

CU_NS_BEGIN

struct NetInfo
{
	enum
	{
		T_NONE,
		T_ACCEPT,
		T_CONNECT,
	};
	uint	mode;
	uint	type;	// �Զ�������
	String	host;
	NetInfo() :mode(T_NONE), type(0){}
};
typedef std::vector<NetInfo> NetInfoVec;

struct NetConfig
{
	uint	services;
	uint	workers;
	NetInfoVec infos;
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

	virtual bool init();
	virtual void quit();
	virtual void loop();
	virtual void update();

	virtual void onError(Session* sess, error_t ec){}
	virtual void onAccept(Acceptor* acceptor, socket_t sock);
	virtual void onConnect(Session* sess){}
	virtual bool onPacket(Session* sess, IPacket* msg);

protected:
	typedef HashMap<uint, Session*>		SessionMap;
	// ���ݲ�ͬ����ע��acceptor
	typedef HashMap<uint, Acceptor*>	AcceptorMap;
	typedef HashMap<uint, Session*>		ConnectorMap;
	typedef std::list<Session*>			SessionList;
	typedef std::queue<NetEvent*>	EventQueue;
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
	NetConfig		m_config;
};

extern NetService* gNetService;

CU_NS_END
