#pragma once
#include "SocketChannel.h"
#include "Protocol.h"

CU_NS_BEGIN

class CU_API Session
{
public:
	Session(uint id, IOService* service, IProtocol* proto, socket_t sock = INVALID_SOCKET);
	~Session();

	uint getId() const { return m_id; }
	uint getType() const { return m_type; }
	void setType(uint type) { m_type = type; }

	void setData(void* data);
	void* getData() { return m_data; }

	SocketChannel* getChannel() { return m_channel; }
	Buffer& getReader() { return m_channel->getReader(); }

	void reconnect() { m_channel->reconnect(); }
	void connect(const SocketAddress& addr) { m_channel->connect(addr); }
	void send(const Buffer& buf) { m_channel->send(buf); }
	void send(const char* str);

	void shutdown(int how) { m_channel->shutdown(how); }
	void close() { m_channel->close(); }

private:
	void notify(uint8_t type);

private:
	uint		m_id;
	void*		m_data;
	uint		m_type;
	IProtocol*	m_proto;
	SocketChannel* m_channel;
};

CU_NS_END
