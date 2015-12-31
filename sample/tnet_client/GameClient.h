#pragma once
#include "NetService.h"
using namespace cute;

class Client : public NetService
{
public:
	Client();

	IProtocol* getProtocol(int type);
	bool onEvent(NetEvent* ev);
	void onConnect(Session* sess);
	void onError(Session* sess, error_t ec);
};

extern Client gClient;