#pragma once
#include "Cute/Server.h"
using namespace Cute;

class GameServer : public Server
{
public:
	GameServer();
	~GameServer();

	bool setup();
private:
};

extern GameServer gGameServer;
