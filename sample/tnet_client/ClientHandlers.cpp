#include "ClientHandlers.h"
#include "HandlerMgr.h"

ClientHandlers gClientHandlers;

template<typename F>
inline void add(F fun)
{
	HandlerMgr::instance().add(fun, &gClientHandlers);
}

ClientHandlers::ClientHandlers()
{
	init();
}

void ClientHandlers::init()
{
	add(&ClientHandlers::onChat);
}

int ClientHandlers::onChat(ChatMsg* msg, Session* sess)
{
	return 0;
}