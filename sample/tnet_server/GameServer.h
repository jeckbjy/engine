#pragma once
#include "NetService.h"
using namespace cute;
//#include "Message.h"

class GameServer : public NetService
{
public:
	GameServer();
};

extern GameServer gGame;