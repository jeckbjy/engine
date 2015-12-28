#pragma once
#include "NetService.h"
using namespace cute;

class GameServer : public NetService
{
public:
	GameServer();

	IProtocol* getProtocol(int type);
	bool onEvent(NetEvent* ev);
};

extern GameServer gGame;