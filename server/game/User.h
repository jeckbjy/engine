#pragma once
#include "Cute/Session.h"
using namespace Cute;

class Player;
class User
{
public:
	User();
	~User();

	void onError();

private:
	Player*			m_player;
};
