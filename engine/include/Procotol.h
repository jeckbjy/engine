#pragma once
#include "API.h"

CU_NS_BEGIN

class Session;
// ��ϢЭ��
class IProcotol
{
public:
	virtual ~IProcotol(){}
	virtual void* create(){ return 0; }
	virtual void  destroy(void*){}
	virtual void  process(Session* sess) = 0;
};

// �����Զ�����Ϣ��Э��
class PacketProtocol : public IProcotol
{
public:
	void process(Session* sess);
};


CU_NS_END