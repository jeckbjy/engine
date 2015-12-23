#pragma once
#include "Server/TcpClient.h"
using namespace cute;

class Client : public TcpClient
{
public:
	void onConnect(TcpSession* sess);
	void onError(TcpSession* sess, int code);
};

extern Client gClient;