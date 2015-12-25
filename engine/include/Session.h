#pragma once
#include "SocketChannel.h"
#include "Protocol.h"

CU_NS_BEGIN

class Session : public SocketChannel
{
public:
	Session(IOService* service, uint id, uint type = 0, IProtocol * proto = PacketProtocol::InstancePtr());
	~Session();

	void shutdown(int how);
	void close();

	void notify(uint8_t type);

	void setData(void* data);
	void* getData() { return m_data; }

private:
	uint		m_id;
	uint		m_type;
	IProtocol*	m_proto;
	void*		m_data;
};

CU_NS_END
