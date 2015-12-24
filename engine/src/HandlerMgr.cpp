#include "HandlerMgr.h"

CU_NS_BEGIN

HandlerMgr::HandlerMgr()
{

}

HandlerMgr::~HandlerMgr()
{

}

IHandler* HandlerMgr::find(uint msgid)
{
	return 0;
}

void HandlerMgr::add_handler(uint msgid, IHandler* handler)
{

}

CU_NS_END