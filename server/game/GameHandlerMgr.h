#pragma once
#include "Cute/HandlerRegister.h"
using namespace Cute;

class HandlerModule
{

};

// ��ͬģ���Handler
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

// ע��ͻ�����Ϣ
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

// ע��World���ص���Ϣ
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
