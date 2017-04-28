#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class CU_VK_API VK_CommandQueue : public CommandQueue
{
public:
	VK_CommandQueue();
	~VK_CommandQueue();

	void submit(CommandBuffer* cmds, Fence* fence);
	void wait();

private:
	VkQueue			m_queue;
	VkSubmitInfo	m_submit;
};

CUTE_NS_END
