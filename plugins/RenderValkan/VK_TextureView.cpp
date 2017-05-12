#include "VK_TextureView.h"
#include "VK_Texture.h"
#include "VK_Device.h"
#include "VK_Mapping.h"

CUTE_NS_BEGIN

VK_TextureView::VK_TextureView()
	: m_device(NULL)
	, m_texture(NULL)
	, m_view(VK_NULL_HANDLE)
	, m_aspect(0)
{
}

VK_TextureView::~VK_TextureView()
{
	term();
}

bool VK_TextureView::init(VK_Device* device, ITexture* tex, const TextureViewDesc& desc)
{
	term();
	m_device	= device;
	m_texture	= (VK_Texture*)tex;
	m_aspect	= VK_Mapping::getImageAspect(desc.aspect);

	VkImageViewCreateInfo info = {};
	info.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	info.pNext        = nullptr;
	info.flags        = 0;
	info.image        = m_texture->getImage();
	info.viewType     = VK_Mapping::getImageViewType(desc.dimension);
	info.format       = VK_Mapping::getFormat(desc.format);
	info.components.r = VK_Mapping::getComponentSwizzle(desc.components.r);
	info.components.g = VK_Mapping::getComponentSwizzle(desc.components.g);
	info.components.b = VK_Mapping::getComponentSwizzle(desc.components.b);
	info.components.a = VK_Mapping::getComponentSwizzle(desc.components.a);
	info.subresourceRange.aspectMask     = m_aspect;
	info.subresourceRange.baseMipLevel   = desc.mipSlice;
	info.subresourceRange.levelCount     = desc.mipLevels;
	info.subresourceRange.baseArrayLayer = desc.firstArraySlice;
	info.subresourceRange.layerCount     = desc.arraySize;;

	VkResult ret = vkCreateImageView(device->getDevice(), &info, NULL, &m_view);
	return ret == VK_SUCCESS;
}

void VK_TextureView::term()
{
	if (m_device == NULL)
		return;

	VkDevice device = m_device->getDevice();

	if (m_view != VK_NULL_HANDLE)
	{
		vkDestroyImageView(device, m_view, NULL);
		m_view = VK_NULL_HANDLE;
	}

	m_texture = NULL;
	m_device = NULL;
}

CUTE_NS_END
