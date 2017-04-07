#pragma once
#include "Cute/Object.h"
#include "Cute/SocketDefs.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

class IOProxy;
class IOChannel;
class CUTE_CORE_API IORequest : public Object
{
	DECLARE_RTTI(IORequest, Object, OID_ANY);
public:
#if defined(_WIN32)
	struct IOData : public OVERLAPPED
	{
		IORequest* op;
	};
#else
	struct IOData
	{
	};
#endif

public:
	IORequest(IOChannel* channel);
	virtual ~IORequest();

	virtual void perform();

	bool isSuccess() const;

	IOProxy* getProxy() { return m_proxy; }
	IOData*	 getData() { return &m_data; }
	error_t  getCode() { return m_code; }
	void	 setCode(error_t ec) { m_code = ec; }

protected:
	friend class IOLoop;
	typedef RefPtr<IOProxy> ProxyPtr;
	IORequest*	m_next;
	ProxyPtr	m_proxy;
	IOData		m_data;
	error_t		m_code;
};

class CUTE_CORE_API AcceptReq : public IORequest
{
	DECLARE_RTTI(AcceptReq, IORequest, OID_ANY)
public:
	enum { ADDR_LEN = sizeof(sockaddr_in6) + 16 };
	AcceptReq(IOChannel* channel) 
		:IORequest(channel), m_sock(INVALID_SOCKET)
	{}

	void perform();

	void		setSocket(socket_t sock) { m_sock = sock; }
	socket_t	getSocket() const { return m_sock; }
	char*		getBuffer() { return m_buff; }
	int			getLength() { return ADDR_LEN; }

private:
	socket_t	m_sock;
	char		m_buff[ADDR_LEN * 2];
};

class CUTE_CORE_API ConnectReq : public IORequest
{
	DECLARE_RTTI(ConnectReq, IORequest, OID_ANY)
public:
	ConnectReq(IOChannel* channel)
		:IORequest(channel){}
};

class CUTE_CORE_API ReadReq : public IORequest
{
	DECLARE_RTTI(ReadReq, IORequest, OID_ANY)
public:
	ReadReq(IOChannel* channel)
		: IORequest(channel){}
};

class CUTE_CORE_API WriteReq : public IORequest
{
	DECLARE_RTTI(WriteReq, IORequest, OID_ANY)
public:
	WriteReq(IOChannel* channel)
		: IORequest(channel){}
};

CUTE_NS_END
