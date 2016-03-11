#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API  VK_Mapping
{
public:
	static VkFormat getFormat(PixelFormat format);
	static VkBufferUsageFlags getBufferUsage(BIND_FLAG usage);
	static VkImageType getImageType(TexType type);
	static VkImageUsageFlagBits getImageUsage(RES_FLAG usage);
};

CU_NS_END
