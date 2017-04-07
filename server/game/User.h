#pragma once
#include "Cute/TCPConnection.h"
using namespace Cute;

class Player;
class User : public TCPConnection::Listener
{
public:
	User(TCPConnection*	conn);
	~User();

	void onError();

private:
	TCPConnection*	m_conn;
	Player*			m_player;
};
