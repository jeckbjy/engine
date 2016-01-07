#pragma once
#include "API.h"
#include "Singleton.h"

CU_NS_BEGIN

class Session;
// 消息协议,processor
class IProtocol
{
public:
	virtual ~IProtocol(){}
	virtual void* create(){ return 0; }
	virtual void  destroy(void*){}
	virtual void  process(Session* sess) = 0;
};

// 基于自定义消息包协议
class CU_API PacketProtocol : public IProtocol, public Singleton<PacketProtocol>
{
public:
	void process(Session* sess);
};

// 文本行协议,CRLF结束
class CU_API TextProtocol : public IProtocol, public Singleton<TextProtocol>
{
public:
	void process(Session* sess);
};

CU_NS_END
