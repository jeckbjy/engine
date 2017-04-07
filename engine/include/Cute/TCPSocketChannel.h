#pragma once
#include "Cute/SocketChannel.h"
#include "Cute/IORequest.h"

CUTE_NS_BEGIN

class CUTE_CORE_API TCPSocketChannel : public SocketChannel
{
public:
	TCPSocketChannel(IOLoop* loop);
	~TCPSocketChannel();

	void open(socket_t sock);
	void connect(const SocketAddress& addr);
	void send(const BufferList& data);
	void recv();
	void shutdown(int how /* = SHUT_RDWR */);

	void setOption(const ChannelOption& option);
	void getOption(ChannelOption& option);

	SocketAddress localAddress() const;
	SocketAddress peerAddress() const;
	BufferList&	  getReader() { return m_reader; }

protected:
	void doSend();
	void selection(int events);
	void completed(IORequest* req);

private:
	Mutex		m_mutex;
	BufferList	m_reader;
	BufferList	m_writer;
	ReadReq*	m_reqRead;
	WriteReq*	m_reqWrite;
	ConnectReq*	m_reqConnect;
};

//////////////////////////////////////////////////////////////////////////
// Option
//////////////////////////////////////////////////////////////////////////
class CUTE_CORE_API SocketOption : public ChannelOption
{
	DECLARE_RTTI(SocketOption, ChannelOption, OID_ANY)
public:
	SocketOption(int type) :m_type(type){}
	SocketOption(int type, int value) :m_type(type), m_value(value){}
	SocketOption(int type, bool value) : m_type(type), m_value(value ? 1 : 0){}

	int  getType() const { return m_value; }
	void setValue(int value) { m_value = value; }
	int	 getValue() const { return m_value; }
	bool isType(int type) const { return m_type == type; }

public:
	int	m_type;
	int	m_value;
};

class CUTE_CORE_API LingerOption : public ChannelOption
{
	DECLARE_RTTI(LingerOption, ChannelOption, OID_ANY)
public:
	LingerOption();
	LingerOption(bool on, int second);

	void	setValue(bool on, int second);
	void	setValue(const linger& value) { m_linger = value; }
	linger* getValue() { return &m_linger; }
	const linger* getValue() const { return &m_linger; }

	int		getLinger() const { return m_linger.l_linger; }
	bool	isOpen() const { return m_linger.l_onoff != 0; }

private:
	linger m_linger;
};

CUTE_NS_END
