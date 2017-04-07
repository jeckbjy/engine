//! Logic
#include "User.h"

User::User(TCPConnection* conn)
	: m_conn(conn)
	, m_player(NULL)
{

}

User::~User()
{

}

void User::onError()
{
}
