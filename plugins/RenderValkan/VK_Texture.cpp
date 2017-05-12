#include "VK_Texture.h"
#include "VK_Mapping.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

VK_Texture::VK_Texture()
	: m_device(NULL)
	, m_image(VK_NULL_HANDLE)
	, m_memory(VK_NULL_HANDLE)
	, m_aspects(VK_IMAGE_ASPECT_COLOR_BIT)
	, m_state(RESOURCE_STATE_UNKNOWN)
	, m_external(false)
{
}

VK_Texture::~VK_Texture()
{
	term();
}

bool VK_Texture::isSupportFormat(VK_Device* device, const TextureDesc& desc)
{
	RESOURCE_USAGE	usage = desc.usage;
	RESOURCE_LAYOUT	layout = desc.layout;

	VkPhysicalDevice physical = device->getPhysicalDevice();
	assert(physical != VK_NULL_HANDLE);

	VkFormat format = VK_Mapping::getFormat(desc.format);
	VkFormatProperties formatProps;
	vkGetPhysicalDeviceFormatProperties(physical, format, &formatProps);

	if (desc.usage & RESOURCE_USAGE_COLOR_TARGET)
	{
		if (desc.layout == RESOURCE_LAYOUT_LINEAR)
		{
			if ((formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) == 0)
				return false;
		}
		else
		{
			if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) == 0)
				return false;
		}
	}

	if (usage & RESOURCE_USAGE_DEPTH_TARGET)
	{
		if (desc.layout == RESOURCE_LAYOUT_LINEAR)
		{
			if ((formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == 0)
				return false;
		}
		else
		{
			if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == 0)
				return false;
		}
	}

	if (usage & RESOURCE_USAGE_UNORDERD_ACCESS)
	{
		if (layout == RESOURCE_LAYOUT_LINEAR)
		{
			if ((formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT) == 0)
				return false;
		}
		else
		{
			if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT) == 0)
				return false;
		}
	}

	if (usage & RESOURCE_USAGE_CONSTANT_BUFFER)
	{
		if (layout == RESOURCE_LAYOUT_LINEAR)
		{
			if ((formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) == 0)
				return false;
		}
		else
		{
			if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) == 0)
				return false;
		}
	}

	if (usage & RESOURCE_USAGE_INDEX_BUFFER)
		return false;

	if (usage & RESOURCE_USAGE_VERTEX_BUFFER)
		return false;

	if (usage & RESOURCE_USAGE_INDIRECT_BUFFER)
		return false;

	return true;
}

bool VK_Texture::init(VK_Device* device, const TextureDesc& desc)
{
	VkDevice native;
	VkResult ret;

	m_device = device;
	m_device->incRef();
	native = device->getDevice();

	// check support
	if (!isSupportFormat(device, desc))
		return false;

	// create texture
	VkImageCreateInfo info = {};
	info.sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
	info.imageType				= VK_Mapping::getImageType(desc.dimension);;
    info.format                 = VK_Mapping::getFormat(desc.format);
    info.extent.width           = desc.width;
    info.extent.height          = desc.height;
    info.extent.depth           = (desc.dimension == RESOURCE_DIMENSION_TEXTURE3D) ? desc.depthOrArraySize: 1;
    info.mipLevels              = desc.mipLevels;
    info.arrayLayers            = (desc.dimension != RESOURCE_DIMENSION_TEXTURE3D) ? desc.depthOrArraySize : 1;
    info.samples                = VK_Mapping::getSampleCountFlagBits(desc.sampleCount);
    info.tiling                 = (desc.layout == RESOURCE_LAYOUT_LINEAR) ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
    info.usage                  = VK_Mapping::getImageUsage(desc.usage); //ToNativeImageUsage(pDesc->Usage);
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED;

	// 
	ret = vkCreateImage(native, &info, NULL, &m_image);
	if (ret != VK_SUCCESS)
		return false;

	// get requirements
	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(native, m_image, &requirements);

	// alloc memory
	bool isMappable = desc.layout == RESOURCE_LAYOUT_LINEAR;
	VkMemoryPropertyFlags flags = VK_Mapping::getMemoryPropertyFlags(desc.heapProperty.cpuPageProperty, isMappable);

	if (!device->alloc(m_memory, requirements, flags))
		return false;

	// bind memory
	ret = vkBindImageMemory(native, m_image, m_memory, 0);
	if (ret != VK_SUCCESS)
		return false;

	// set aspect
	if (desc.format == RESOURCE_FORMAT_D24_UNORM_S8_UINT)
		m_aspects = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	else if (desc.format == RESOURCE_FORMAT_D16_UNORM || desc.format == RESOURCE_FORMAT_D32_FLOAT)
		m_aspects = VK_IMAGE_ASPECT_DEPTH_BIT;
	else
		m_aspects = VK_IMAGE_ASPECT_COLOR_BIT;

	// TODO:set state

	//
	m_external = false;
	return true;
}

void VK_Texture::term()
{
	if (m_device == NULL)
		return;

	VkDevice native = m_device->getDevice();

	if (!m_external)
	{
		if (m_image != VK_NULL_HANDLE)
		{
			vkDestroyImage(native, m_image, NULL);
			m_image = VK_NULL_HANDLE;
		}

		if (m_memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(native, m_memory, NULL);
			m_memory = VK_NULL_HANDLE;
		}
	}
	else
	{
		m_image = VK_NULL_HANDLE;
		m_memory = VK_NULL_HANDLE;
	}

	m_device->decRef();
}

void* VK_Texture::map()
{
	if (m_memory == VK_NULL_HANDLE)
		return NULL;

	VkDevice device;
	VkResult result;
	void*	 data;

	device = m_device->getDevice();
	result = vkMapMemory(device, m_memory, 0, m_desc.width, 8, &data);
	if (result != VK_SUCCESS)
		return NULL;

	return data;
}

void VK_Texture::unmap()
{
	if (m_memory == VK_NULL_HANDLE)
		return;

	VkDevice device = m_device->getDevice();
	vkUnmapMemory(device, m_memory);
}

bool VK_Texture::getSubresourceLayout(SubresourceLayout& result, uint32 subresource)
{
	if (m_desc.layout == RESOURCE_LAYOUT_LINEAR)
	{
		VkDevice device = m_device->getDevice();

		VkImageSubresource subres = {};
		subres.aspectMask = m_aspects;

		VkSubresourceLayout layout = {};
		vkGetImageSubresourceLayout(device, m_image, &subres, &layout);

		result.offset	= layout.offset;
		result.size		= layout.size;
		if (m_desc.dimension == RESOURCE_DIMENSION_TEXTURE3D)
			result.slicePitch = layout.depthPitch;
		else
			result.slicePitch = layout.arrayPitch;

		if (layout.rowPitch != 0)
		{
			result.rowPitch = layout.rowPitch;
			result.rowCount = layout.size / layout.rowPitch;
		}
		else
		{
			result.rowPitch = result.slicePitch / m_desc.height;
			result.rowCount = m_desc.height;
		}
	}
	else
	{

	}

	return true;
}

VkImage VK_Texture::getImage() const
{
	return m_image;
}

//VK_Texture::VK_Texture(VK_Device* device, const TextureDesc& desc)
//	: Texture(desc)
//	, m_device(device)
//{
//	uint32_t arrayLayers, depth;
//	if (desc.type == TEX_3D)
//	{
//		arrayLayers = 1;
//		depth = desc.depthOrArraySize;
//	}
//	else
//	{
//		arrayLayers = desc.depthOrArraySize;
//		depth = 1;
//	}
//
//	VkImageCreateInfo info;
//	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//	info.pNext = NULL;
//	info.imageType = VK_Mapping::getImageType(desc.type);
//	info.format = VK_Mapping::getFormat(desc.format);
//	info.usage = VK_Mapping::getImageUsage(desc.usage);
//	info.mipLevels = desc.mipLevels;
//	info.arrayLayers = arrayLayers;
//	info.tiling = (VkImageTiling)desc.tiling;
//	info.flags = 0;
//	info.extent = { desc.width, desc.height, depth };
//
//	VK_CHECK(vkCreateImage(m_device->native(), &info, NULL, &m_image), "vkCreateImage fail!");
//
//	// memory
//	VkMemoryRequirements mem_reqs;
//	vkGetImageMemoryRequirements(m_device->native(), m_image, &mem_reqs);
//	m_device->allocMemory(m_memory, mem_reqs.size, mem_reqs.memoryTypeBits);
//	VK_CHECK(vkBindImageMemory(m_device->native(), m_image, m_memory, 0), "vkBindImageMemory fail!");
//
//	// view ??
//}
//
//VK_Texture::~VK_Texture()
//{
//	if (m_image != VK_NULL_HANDLE)
//		vkDestroyImage(m_device->native(), m_image, NULL);
//	if (m_memory != VK_NULL_HANDLE)
//		vkFreeMemory(m_device->native(), m_memory, NULL);
//}
//
//void* VK_Texture::map(PixelData& data, MAP_FLAG flag, uint level /* = 0 */, uint face /* = 0 */)
//{
//	return NULL;
//}
//
//void VK_Texture::unmap()
//{
//
//}

CUTE_NS_END
