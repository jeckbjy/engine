//! Main
#include "GameServer.h"

GameServer gGameServer;

GameServer::GameServer()
{

}

GameServer::~GameServer()
{

}

bool GameServer::setup()
{
	if (!Server::setup())
		return false;

	// ��ȡ����
	//listen(CONN_TYPE_ADMIN, 12001);
	//listen(newFactory<ClientConnection>(), 12002);
	return true;
}
