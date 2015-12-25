#pragma once
#include "Message.h"
#include "Session.h"

class ServerHandlers
{
public:
	ServerHandlers();

	void init();

	void onChat(ChatMsg* msg, Session* sess);
};