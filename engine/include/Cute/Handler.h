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
1:解码处理
2:权限校验:相同类型Session或者Any类型的Session才能执行
3:是否允许异步执行
4:返回值处理:0表示成功，负数表示需要延迟处理，正数表示错误码
5:性能统计,预警
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
	int  m_type;	// 允许执行的session类型,0表示无限制
};

CUTE_NS_END
