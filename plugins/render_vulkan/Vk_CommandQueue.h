#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_CommandQueue : public CommandQueue
{
public:
	VK_CommandQueue();
	~VK_CommandQueue();

	void submit();
	void wait();

private:
	VkQueue			m_queue;
	VkSubmitInfo	m_submit;
};

CU_NS_END
