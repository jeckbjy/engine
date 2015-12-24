#pragma once
#include "API.h"

CU_NS_BEGIN

struct IPacket;
class Session;
class NetService;
struct NetEvent
{
	virtual ~NetEvent(){}
	virtual bool process(NetService* service) = 0;
};

struct ConnectEvent : NetEvent
{
	Session* sess;
	bool process(NetService* service);
};

struct ErrorEvent : NetEvent
{
	Session* sess;
	error_t	 code;
	bool process(NetService* serivce);
};

struct PacketEvent : NetEvent
{
	Session* sess;
	IPacket* msg;
	bool process(NetService* serivce);
};

CU_NS_END