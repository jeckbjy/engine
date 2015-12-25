#pragma once
#include "Message.h"
#include "Session.h"

class ClientHandlers
{
public:
	ClientHandlers();

	void init();

	void onChat(ChatMsg* msg, Session* sess);
};