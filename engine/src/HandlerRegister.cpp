//! Server
#include "Cute/HandlerRegister.h"
#include "Cute/HandlerService.h"

CUTE_NS_BEGIN

HandlerRegister::HandlerRegister()
{
}

HandlerRegister::~HandlerRegister()
{
}

void HandlerRegister::regist(uint msgid, Handler* handler)
{
	HandlerService::instance().regist(msgid, handler);
}


CUTE_NS_END
