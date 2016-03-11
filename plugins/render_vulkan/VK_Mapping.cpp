#include "VK_Mapping.h"

CU_NS_BEGIN

VkFormat VK_Mapping::getFormat(PixelFormat format)
{
	return VK_FORMAT_UNDEFINED;
}

VkBufferUsageFlags VK_Mapping::getBufferUsage(BIND_FLAG usage)
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

VkImageType VK_Mapping::getImageType(TexType type)
{
	switch (type)
	{
	case TEX_1D: return VK_IMAGE_TYPE_1D;
	case TEX_2D: return VK_IMAGE_TYPE_2D;
	case TEX_3D: return VK_IMAGE_TYPE_3D;
	}
	return VK_IMAGE_TYPE_2D;
}

VkImageUsageFlagBits VK_Mapping::getImageUsage(RES_FLAG usage)
{
	return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
}

CU_NS_END
