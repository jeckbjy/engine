#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKCommandQueue : public CommandQueue
{
public:
	VKCommandQueue();
	~VKCommandQueue();

private:
	VkQueue m_queue;
};

CU_NS_END