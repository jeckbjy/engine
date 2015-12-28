#pragma once
#include "Message.h"
#include "Session.h"

class ClientHandlers
{
public:
	ClientHandlers();

	void init();

	int onChat(ChatMsg* msg, Session* sess);
};