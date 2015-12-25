#include "ServerHandlers.h"
#include "HandlerMgr.h"

ServerHandlers gServerHandlers;

template<typename F>
inline void add(F fun)
{
	HandlerMgr::instance()->add(fun, &gServerHandlers);
}

ServerHandlers::ServerHandlers()
{
	init();
}

void ServerHandlers::init()
{
	add(&ServerHandlers::onChat);
}

void ServerHandlers::onChat(ChatMsg* msg, Session* sess)
{

}