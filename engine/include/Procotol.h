#pragma once
#include "API.h"

CU_NS_BEGIN

class Session;
// 消息协议
class IProcotol
{
public:
	virtual ~IProcotol(){}
	virtual void* create(){ return 0; }
	virtual void  destroy(void*){}
	virtual void  process(Session* sess) = 0;
};

// 基于自定义消息包协议
class PacketProtocol : public IProcotol
{
public:
	void process(Session* sess);
};


CU_NS_END