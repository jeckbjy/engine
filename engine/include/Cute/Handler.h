//! Server
#pragma once
#include "Cute/Foundation.h"
#include "Cute/LogicEvent.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

class Packet;
class Session;
class CUTE_CORE_API PacketEvent : public LogicEvent
{
public:
	PacketEvent(Session* sess, Packet* msg);
	~PacketEvent();

	void process();

	Session* getSession() { return m_sess; }
	Packet*	 getPacket() { return m_msg; }

protected:
	Session* m_sess;
	Packet*	 m_msg;
};

/*
1:���봦��
2:Ȩ��У��:��ͬ����Session����Any���͵�Session����ִ��
3:�Ƿ������첽ִ��
4:����ֵ����:0��ʾ�ɹ���������ʾ��Ҫ�ӳٴ���������ʾ������
5:����ͳ��,Ԥ��
*/
class CUTE_CORE_API Handler
{
public:
	Handler();
	virtual ~Handler(){}

	void process(PacketEvent* ev);

	virtual int 	handle(PacketEvent* ev) = 0;
	virtual Packet* decode(BufferList& buffer) = 0;

	void setAsync(bool async) { m_async = async; }
	void setType(int type){ m_type = type; }

	bool canAsync() const { return m_async; }
	bool canAccept(int type) const { return m_type == 0 || m_type == type; }

protected:
	bool m_async;
	int  m_type;	// ����ִ�е�session����,0��ʾ������
};

CUTE_NS_END
