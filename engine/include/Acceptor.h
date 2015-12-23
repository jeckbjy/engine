#pragma once
#include "AcceptChannel.h"

CU_NS_BEGIN

class CU_API Acceptor : public AcceptChannel
{
public:
	Acceptor(IOService* service, uint type = 0);
	~Acceptor();

	void completed(socket_t sock);

private:
	uint	m_type;
};

CU_NS_END