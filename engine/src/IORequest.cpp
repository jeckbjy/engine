//! IO
#include "Cute/IORequest.h"
#include "Cute/IOChannel.h"

CUTE_NS_BEGIN

// 暂时不考虑引用计数
IORequest::IORequest(IOChannel* channel)
	: m_next(0)
	, m_proxy(channel->getProxy())
	, m_code(0)
{
#ifdef _WIN32
	memset(&m_data, 0, sizeof(IOData));
	m_data.op = this;
#endif
}

IORequest::~IORequest()
{
}

void IORequest::perform()
{
}

bool IORequest::isSuccess() const
{
	return m_code == 0;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void AcceptReq::perform()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	if (isSuccess())
	{
		SOCKET update_ctx = (SOCKET)m_proxy->handle();
		::setsockopt(m_sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&update_ctx, sizeof(SOCKET));
	}
	else
	{
		::closesocket(m_sock);
		m_sock = INVALID_SOCKET;
	}
#endif
}

CUTE_NS_END
