#pragma once
#include "NetService.h"
using namespace cute;

class Client : public NetService
{
public:
	Client();

	IProtocol* getProtocol(int type);
	void onConnect(Session* sess);
	void onError(Session* sess);
};

extern Client gClient;