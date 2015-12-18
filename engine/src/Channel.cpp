#include "Channel.h"
#include "EventLoop.h"

CU_NS_BEGIN

Channel::Channel(EventLoop* loop /* = NULL */)
	: m_loop(loop)
	, m_attached(false)
{
}

Channel::~Channel()
{
	detach();
}

void Channel::attach()
{
	if (m_attached || !m_loop)
		return;
	if (m_loop->attach(this))
		m_attached = true;
}

void Channel::detach()
{
	if (m_attached)
	{
		m_loop->detach(this);
		m_attached = false;
	}
}

CU_NS_END
