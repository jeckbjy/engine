#include "GameServer.h"
#include "Log.h"

GameServer gGame;

GameServer::GameServer()
{
	m_config.add_host(NetInfo::T_ACCEPT, "0.0.0.0:6666");
}

IProtocol* GameServer::getProtocol(int type)
{
	return TextProtocol::InstancePtr();
}

bool GameServer::onEvent(NetEvent* ev)
{
	if (ev->isKindOf<TextEvent>())
	{
		TextEvent* tev = (TextEvent*)ev;
		tev->sess->send("server chat\r\n");
		LOG_TRACE("%s", tev->text.toString().c_str());
	}

	return true;
}