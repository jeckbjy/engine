#include "IOOperation.h"
#include "Channel.h"

CU_NS_BEGIN

// 暂时不考虑引用计数
IOOperation::IOOperation(Channel* channel)
:channel(channel), next(0), code(0)
{
#ifdef CU_OS_WIN
	memset(&data, 0, sizeof(IOData));
	data.op = this;
#endif
	channel->retain();
}

IOOperation::~IOOperation()
{
	channel->release();
}

CU_NS_END