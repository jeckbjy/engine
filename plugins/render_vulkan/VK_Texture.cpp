#include "VK_Texture.h"
#include "VK_Mapping.h"
#include "VK_Device.h"

CU_NS_BEGIN

inline static VkImageType getImageType(TexType type)
{
	switch (type)
	{
	case TEX_1D: return VK_IMAGE_TYPE_1D;
	case TEX_2D: return VK_IMAGE_TYPE_2D;
	case TEX_3D: return VK_IMAGE_TYPE_3D;
	}
	return VK_IMAGE_TYPE_2D;
}

VK_Texture::VK_Texture(VK_Device* device, const TextureDesc& desc)
	: Texture(desc)
	, m_device(device)
{
	uint32_t arrayLayers, depth;
	if (desc.type == TEX_3D)
	{
		arrayLayers = 1;
		depth = desc.depthOrArraySize;
	}
	else
	{
		arrayLayers = desc.depthOrArraySize;
		depth = 1;
	}

	VkImageCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.pNext = NULL;
	info.imageType = VK_Mapping::getImageType(desc.type);
	info.format = VK_Mapping::getFormat(desc.format);
	info.usage = VK_Mapping::getImageUsage(desc.usage);
	info.mipLevels = desc.mipLevels;
	info.arrayLayers = arrayLayers;
	info.tiling = (VkImageTiling)desc.tiling;
	info.flags = 0;
	info.extent = { desc.width, desc.height, depth };

	VK_CHECK(vkCreateImage(m_device->native(), &info, NULL, &m_image), "vkCreateImage fail!");

	// memory
	VkMemoryRequirements mem_reqs;
	vkGetImageMemoryRequirements(m_device->native(), m_image, &mem_reqs);
	m_device->allocMemory(m_memory, mem_reqs.size, mem_reqs.memoryTypeBits);
	VK_CHECK(vkBindImageMemory(m_device->native(), m_image, m_memory, 0), "vkBindImageMemory fail!");

	// view ??
}

VK_Texture::~VK_Texture()
{
	if (m_image != VK_NULL_HANDLE)
		vkDestroyImage(m_device->native(), m_image, NULL);
	if (m_memory != VK_NULL_HANDLE)
		vkFreeMemory(m_device->native(), m_memory, NULL);
}

void* VK_Texture::map(PixelData& data, MAP_FLAG flag, uint level /* = 0 */, uint face /* = 0 */)
{
	return NULL;
}

void VK_Texture::unmap()
{

}

CU_NS_END
