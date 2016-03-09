#include "VK_Mapping.h"

CU_NS_BEGIN

VkBufferUsageFlags VKMapping::getBufferUsage(BIND_FLAG usage)
{
	switch (usage)
	{
	case BIND_VERTEX_BUFFER:
		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case BIND_INDEX_BUFFER:
		return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	case BIND_UNIFORM_BUFFER:
		return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	}

	return 0;
}

CU_NS_END
