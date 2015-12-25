#pragma once
#include "Session.h"

class Player
{
public:
	Player(Session* sess);
	~Player();

private:
	Session* m_sess;
};
