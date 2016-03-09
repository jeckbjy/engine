#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API  VKMapping
{
public:
	static VkBufferUsageFlags getBufferUsage(BIND_FLAG usage);
};

CU_NS_END
