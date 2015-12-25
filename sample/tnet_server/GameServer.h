#pragma once
#include "NetService.h"
using namespace cute;

class GameServer : public NetService
{
public:
	GameServer();
};

extern GameServer gGame;