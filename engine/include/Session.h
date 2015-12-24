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
	uint		m_type;
	uint		m_id;
	IProcotol*	m_proto;
	void*		m_data;
};

CU_NS_END
