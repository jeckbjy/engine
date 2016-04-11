#include "Channel.h"
#include "IOService.h"

CU_NS_BEGIN

Channel::Channel(IOService* service /* = NULL */)
: m_serivce(service)
, m_code(0)
{
}

Channel::~Channel()
{
}

//void Channel::attach()
//{
//	if (m_attached || !m_serivce)
//		return;
//	if (m_serivce->attach(this))
//		m_attached = true;
//}
//
//void Channel::detach()
//{
//	if (m_attached)
//	{
//		m_serivce->detach(this);
//		m_attached = false;
//	}
//}

CU_NS_END
