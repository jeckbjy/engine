#pragma once
#include "Object.h"

CU_NS_BEGIN

class Channel;
class CU_API IOOperation : public Object
{
	DECLARE_RTTI(IOOperation, Object, "IOOP");
#ifdef CU_OS_WIN
	struct IOData : OVERLAPPED
	{
		IOOperation* op;
	};
#endif

#ifdef CU_OS_WIN
	IOData data;
#endif
	Channel* channel;
	IOOperation* next;
	error_t code;
	IOOperation(Channel* channel);
	virtual ~IOOperation();
	bool success() const { return code == 0; }
};

class CU_API SyncOperation : public IOOperation
{
	DECLARE_RTTI(SyncOperation, IOOperation, "IOPS");
public:
	enum
	{
		OP_INPUT	= 0x01,
		OP_OUTPUT	= 0x02,
		//OP_PRI		= 0x04,
	};
	uint8_t flags;
	SyncOperation(Channel* channel, uint8_t flags)
		:IOOperation(channel), flags(flags)
	{
	}
	bool isInput() const { return (flags & OP_INPUT) != 0; }
	bool isOutput() const { return (flags & OP_OUTPUT) != 0; }
};

// 同步Socket操作
class CU_API SocketOperation : public IOOperation
{
	DECLARE_RTTI(SocketOperation, IOOperation, "IOPS");
public:
	enum
	{
		OP_ERROR,
		OP_CONNECT,
		OP_READ,
		OP_WRITE,
	};
	uint8_t type;
	SocketOperation(Channel* channel, uint8_t type)
		:IOOperation(channel), type(type)
	{}
};

// 只能是异步，accept，iocp无法实现
class CU_API AcceptOperation : public IOOperation
{
	DECLARE_RTTI(AcceptOperation, IOOperation, "IOPA");
public:
	enum { ADDR_LEN = sizeof(sockaddr_in6) + 16 };
	socket_t sock;
	char buff[ADDR_LEN];
	AcceptOperation(Channel* channel)
		: IOOperation(channel)
		, sock(INVALID_SOCKET)
	{
	}
	virtual ~AcceptOperation(){}
};

CU_NS_END