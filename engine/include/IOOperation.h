#pragma once
#include "Object.h"

CU_NS_BEGIN

class Channel;
class CU_API IOOperation : public Object
{
	DECLARE_RTTI(IOOperation, Object, "IOOP");
#ifdef CU_OS_WIN
	struct IOData : public OVERLAPPED
#else
	struct IOData
#endif
	{
		IOOperation* op;
	};
	IOData data;
	Channel* channel;
	IOOperation* next;
	error_t code;
	IOOperation(Channel* channel);
	virtual ~IOOperation();
	bool success() const { return code == 0; }
};

// ͬ��Socket����
class CU_API SocketOperation : public IOOperation
{
	DECLARE_RTTI(SocketOperation, IOOperation, "IOPS");
public:
	enum
	{
		OP_CONNECT,
		OP_READ,
		OP_WRITE,
	};
	uint8_t type;
	SocketOperation(Channel* channel, uint8_t type)
		:IOOperation(channel), type(type)
	{}
};

// ֻ�����첽��accept��iocp�޷�ʵ��
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
};

CU_NS_END