#pragma once
#include "API.h"
#include "Buffer.h"
#include "Object.h"

CU_NS_BEGIN

struct IPacket;
class Session;
class NetService;

struct CU_API NetEvent : public Object
{
	DECLARE_RTTI(NetEvent, Object, "NEVT");
	virtual ~NetEvent(){}
	virtual bool process(NetService* service);
};

class Acceptor;
struct CU_API AcceptEvent : public NetEvent
{
	DECLARE_RTTI(AcceptEvent, NetEvent, "ANEV");
	Acceptor* m_acceptor;
	socket_t m_sock;
	AcceptEvent(Acceptor* acceptor, socket_t sock) :m_acceptor(acceptor), m_sock(sock){}
	bool process(NetService* service);
};

struct CU_API ConnectEvent : NetEvent
{
	DECLARE_RTTI(ConnectEvent, NetEvent, "CNEV");
	Session* sess;
	bool process(NetService* service);
};

struct CU_API ErrorEvent : NetEvent
{
	DECLARE_RTTI(ErrorEvent, NetEvent, "ENEV");
	Session* sess;
	error_t	 code;
	bool process(NetService* service);
};

struct CU_API PacketEvent : NetEvent
{
	DECLARE_RTTI(PacketEvent, NetEvent, "PNEV");
	Session* sess;
	IPacket* msg;
	bool process(NetService* service);
};

struct CU_API TextEvent : NetEvent
{
	DECLARE_RTTI(TextEvent, NetEvent, "TNEV");
	Session* sess;
	Buffer text;
};

CU_NS_END