#pragma once
#include "Cute/Object.h"
#include "Cute/RefPtr.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

class IOLoop;
class IOProxy;
class IORequest;
class SocketAddress;
class CUTE_CORE_API IOChannel : public Object
{
	DECLARE_RTTI(IOChannel, Object, OID_ANY);
public:
	IOChannel(IOLoop* loop);
	virtual ~IOChannel();

	void	 close();
	handle_t handle() const;
	socket_t socket() const;
	IOProxy* getProxy();
	IOLoop*	 getLoop();
	void	 setLoop(IOLoop* loop);

	bool	isOpen() const;
	bool	isRegistered() const;
	bool	isBound() const;
	bool	isConnecting() const;
	bool	isClosing() const;
	bool	isActive() const;
	bool	isLocal() const;
	bool	isSocket() const;
	bool	isConnector() const;

	void	setClosing(bool flag);
protected:
	void	modify(int events);
	void	bind(const SocketAddress& addr, bool reuse);

	void	attach();
	void	detach();

	bool	hasFlag(uint32_t mask) const;
	void	setFlag(uint32_t mask, bool value);
	void	setSocket(socket_t sock);

protected:
	virtual void selection(int events) = 0;
	virtual void completed(IORequest* req) = 0;

protected:
	friend class IOLoop;
	friend class IOProxy;
	friend class IORequest;

	typedef RefPtr<IOProxy> ProxyPtr;

	enum SelectionEvent
	{
		EVENT_IN	= 0x01,
		EVENT_OUT	= 0x02,
	};

	enum
	{
		MASK_LOCAL		= 0x0001,
		MASK_SOCKET		= 0x0002,
		MASK_CONNECTOR	= 0x0004,

		MASK_REGISTER	= 0x0010,
		MASK_BOUND		= 0x0020,
		MASK_ACTIVE		= 0x0040,
		MASK_CONNECTING	= 0x0080,
		MASK_CLOSING	= 0x0100,

		MASK_STATUS		= 0xFFF0,
	};

	ProxyPtr	m_proxy;
	IOLoop*		m_loop;
	handle_t	m_handle;
	uint32		m_flag;		// 标志位
	error_t		m_error;	// 最后一次错误码
#ifdef CUTE_OS_FAMILY_POSIX
	int			m_events;
#endif
};

// inlines
inline bool IOChannel::hasFlag(uint32_t mask) const
{
	return (m_flag & mask) != 0;
}

inline void IOChannel::setFlag(uint32_t mask, bool value)
{
	if (value)
		m_flag |= mask;
	else
		m_flag &= ~mask;
}

inline bool IOChannel::isOpen() const
{
	return handle() != INVALID_HANDLE_VALUE;
}

inline bool IOChannel::isRegistered() const
{
	return hasFlag(MASK_REGISTER);
}

inline bool IOChannel::isBound() const
{
	return hasFlag(MASK_BOUND);
}

inline bool IOChannel::isConnecting() const
{
	return hasFlag(MASK_CONNECTING);
}

inline bool IOChannel::isClosing() const
{
	return hasFlag(MASK_CLOSING);
}

inline bool IOChannel::isActive() const
{
	return hasFlag(MASK_ACTIVE);
}

inline bool IOChannel::isLocal() const
{
	return hasFlag(MASK_LOCAL);
}

inline bool IOChannel::isSocket() const
{
	return hasFlag(MASK_SOCKET);
}

inline bool IOChannel::isConnector() const
{
	return hasFlag(MASK_CONNECTOR);
}

inline void IOChannel::setClosing(bool flag)
{
	setFlag(MASK_CLOSING, flag);
}

//////////////////////////////////////////////////////////////////////////
// IOProxy:引用计数，弱引用Channel,perform会校验channel有效性
//////////////////////////////////////////////////////////////////////////
class CUTE_CORE_API IOProxy : public SafeRef<IOProxy>
{
public:
	IOProxy(IOChannel* channel);
	void reset();
	void completed(IORequest* op);
	handle_t handle();

private:
	Mutex		m_mutex;
	IOChannel*	m_channel;
};

class ChannelOption : public Object
{
	DECLARE_RTTI(ChannelOption, Object, OID_ANY)
public:
	virtual ~ChannelOption(){}
};

CUTE_NS_END
