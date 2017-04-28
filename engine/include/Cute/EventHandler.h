//! Server
#pragma once
#include "Cute/Foundation.h"
#include "Cute/EventBase.h"
#include "Cute/BufferList.h"

CUTE_NS_BEGIN

/*
1:���봦��
2:Ȩ��У��:��ͬ����Session����Any���͵�Session����ִ��
3:�Ƿ������첽ִ��
4:����ֵ����:0��ʾ�ɹ���������ʾ��Ҫ�ӳٴ���������ʾ������
5:����ͳ��,Ԥ��
*/
class Packet;
class PacketEvent;
class CUTE_CORE_API EventHandler
{
public:
	EventHandler();
	virtual ~EventHandler(){}

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
