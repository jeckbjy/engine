#include "IOOperation.h"
#include "Channel.h"

CU_NS_BEGIN

IOOperation::IOOperation(Channel* channel)
:channel(channel), next(0), code(0)
{
	memset(&data, 0, sizeof(IOData));
	channel->retain();
}

IOOperation::~IOOperation()
{
	channel->release();
}

CU_NS_END