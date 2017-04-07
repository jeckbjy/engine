#pragma once
#include "Cute/ServerChannel.h"

CUTE_NS_BEGIN

class AcceptReq;
class CUTE_CORE_API TCPServerChannel : public ServerChannel
{
public:
	TCPServerChannel(IOLoop* loop);
	~TCPServerChannel();

	void listen(const SocketAddress& addr);

	void setOption(const ChannelOption& option);
	void getOption(ChannelOption& option);

	socket_t native() const;
private:
	void accept();
	void selection(int events);
	void completed(IORequest* req);

private:
	int			m_backlog;
	int			m_family;
	AcceptReq*	m_req;
};

inline socket_t TCPServerChannel::native() const
{
	return (socket_t)m_handle;
}

class BacklogOption : public ChannelOption
{

};

CUTE_NS_END
