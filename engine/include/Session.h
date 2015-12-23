#pragma once
#include "SocketChannel.h"
#include "Procotol.h"

CU_NS_BEGIN

class Session : public SocketChannel
{
public:
	Session();
	~Session();

	void shutdown(int how);
	void close();

	void notify(uint8_t type);

private:
	IProcotol*		m_proto;
	SocketChannel*	m_channel;
};

CU_NS_END