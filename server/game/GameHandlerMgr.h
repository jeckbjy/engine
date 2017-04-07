#pragma once
#include "Cute/HandlerMgr.h"

struct LoginReq : public Packet
{

};

struct LoginRsp : public Packet
{

};

struct InitDataReq : public Packet
{

};

struct InitDataRsq : public Packet
{
};

class GameHandlerMgr
{
public:
	void setup()
	{
		// ֧�ֶ��?
		add(&onLogin);
		add(&onInitData);
	}

	void onLogin(LoginReq* msg, TCPConnection* conn)
	{
		if (getUser())
		{
			// ����Ͷ��
			post(msg);
			return;
		}

		//
	}

	void onInitData(InitDataReq* msg, User* player)
	{

	}


private:

};

class Handler
{
public:
	virtual void handle(PacketEvent* ev);
};

//// ÿ��ģ��
//class HandlerBase
//{
//protected:
//	virtual void setup();
//	virtual void post(Message* msg);
//};
//
//// ��ͬģ��
//class LoginHandler : public HandlerBase
//{
//public:
//	void setup();
//	void onLogin(LoginReq* req, );
//	void onLogout(LogoutReq* req);
//};
//
////
//class ShipHandler : public HandlerBase
//{
//public:
//	void setup();
//	void onBuy();
//};
//
