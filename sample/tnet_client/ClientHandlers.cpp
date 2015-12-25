#include "ClientHandlers.h"
#include "HandlerMgr.h"

ClientHandlers gClientHandlers;

template<typename F>
inline void add(F fun)
{
	HandlerMgr::instance()->add(fun, &gClientHandlers);
}

ClientHandlers::ClientHandlers()
{
	init();
}

void ClientHandlers::init()
{
	add(&ClientHandlers::onChat);
}

void ClientHandlers::onChat(ChatMsg* msg, Session* sess)
{

}