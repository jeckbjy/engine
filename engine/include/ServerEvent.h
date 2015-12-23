#pragma once
#include "API.h"

CU_NS_BEGIN

class Server;
struct ServerEvent
{
	virtual ~ServerEvent(){}
	virtual bool process(Server* server) = 0;
};

CU_NS_END