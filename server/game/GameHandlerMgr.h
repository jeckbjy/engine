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
		// 支持多个?
		add(&onLogin);
		add(&onInitData);
	}

	void onLogin(LoginReq* msg, TCPConnection* conn)
	{
		if (getUser())
		{
			// 重新投递
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

//// 每个模块
//class HandlerBase
//{
//protected:
//	virtual void setup();
//	virtual void post(Message* msg);
//};
//
//// 不同模块
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
