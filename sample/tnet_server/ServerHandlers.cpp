#include "ServerHandlers.h"
#include "HandlerMgr.h"

ServerHandlers gServerHandlers;

// 显示偏特化,校验sess
template<>
struct cute::SessionConvertor<Player>
{
	static Player* convert(Session* sess)
	{
		return (Player*)sess->getData();
	}
};

template<typename F>
inline void add(F fun)
{
	HandlerMgr::instance().add(fun, &gServerHandlers);
}

ServerHandlers::ServerHandlers()
{
	init();
}

void ServerHandlers::init()
{
	add(&ServerHandlers::onChat);
}

int ServerHandlers::onLogin(LoginMsg* msg, Session* sess)
{
	return HRET_OK;
}

int ServerHandlers::onChat(ChatMsg* msg, Player* player)
{
	return HRET_OK;
}