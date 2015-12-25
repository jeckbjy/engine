#pragma once
#include "API.h"
#include "Singleton.h"

CU_NS_BEGIN

class Session;
// 消息协议
class IProtocol
{
public:
	virtual ~IProtocol(){}
	virtual void* create(){ return 0; }
	virtual void  destroy(void*){}
	virtual void  process(Session* sess) = 0;
};

// 基于自定义消息包协议
class PacketProtocol : public IProtocol, public Singleton<PacketProtocol>
{
public:
	void process(Session* sess);
};


CU_NS_END