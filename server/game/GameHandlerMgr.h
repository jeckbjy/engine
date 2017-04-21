#pragma once
#include "Cute/HandlerRegister.h"
using namespace Cute;

class HandlerModule
{

};

// 不同模块的Handler
class ModuleHandler
{
public:
};

class MiscHandler : public ModuleHandler
{
public:
	void setup();

private:
	void onAdmin();
};

class ShipHandler : public ModuleHandler
{

};

class GuildBattleAllHandler : public ModuleHandler
{
public:

private:
	void onBattle();
};

// 注册客户端消息
class ClientHandler : public HandlerRegister
{
public:
	int type() { return 0; }
	void setup()
	{
		regist(&ClientHandler::onAdmin);
		regist(&ClientHandler::onTransfer);
		regist(&ClientHandler::onLogin);
		//add(&ClientHandler::onLogin);
	}

private:
	int onAdmin(TextPacket* msg);
	int onTransfer(TransferPacket* msg);
	int onLogin();
	int onLogout();
};

// 注册World返回的消息
class WorldHandler : public HandlerRegister
{
public:

};

class WebHandler : public HandlerRegister
{
public:
	void setup()
	{
		regist(&WebHandler::onAdmin);
	}
private:
	int onAdmin(TextPacket* msg);
};
