#pragma once
#include "Server/TcpServer.h"
using namespace cute;
#include "Message.h"

class Server : public TcpServer
{

};

extern Server gGame;