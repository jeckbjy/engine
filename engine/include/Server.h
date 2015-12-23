#pragma once
#include "API.h"
#include "Session.h"
#include "Acceptor.h"
#include "IOService.h"
#include "ServerEvent.h"

CU_NS_BEGIN

// �����̴߳�����Ϣ�ַ�
// ���̴߳���ҵ���߼�,NetService??
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
	// ���ݲ�ͬ����ע��acceptor
	typedef HashMap<uint, Acceptor*>	AcceptorMap;
	typedef std::queue<ServerEvent*>	EventQueue;
	bool			m_quit;
	uint			m_frame;		// ֡Ƶ
	IOServicePool	m_services;
	AcceptorMap		m_acceptors;
	Session*		m_connector;	// ��������
	SessionMap		m_sessions;		// ��������
	EventQueue		m_events;		// ���������Ϣ
	Mutex			m_mutex;		// �¼�������
};

extern Server* gServer;

CU_NS_END
