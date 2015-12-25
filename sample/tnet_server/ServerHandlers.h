#pragma once
#include "Message.h"
#include "Session.h"

class Player;
class ServerHandlers
{
public:
	ServerHandlers();

	void init();

	int onLogin(LoginMsg* msg, Session* sess);
	int onChat(ChatMsg* msg, Player* player);
};