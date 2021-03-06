#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_Texture : public ITexture
{
public:
	VK_Texture();
	~VK_Texture();

	bool init(VK_Device* device, const TextureDesc& desc);
	void term();

	void* map();
	void  unmap();

	bool	getSubresourceLayout(SubresourceLayout& layout, uint32 subresource);
	VkImage getImage() const;

private:
	bool isSupportFormat(VK_Device* device, const TextureDesc& desc);

private:
	VK_Device*			m_device;
	VkImage				m_image;
	VkDeviceMemory		m_memory;
	VkImageAspectFlags	m_aspects;
	RESOURCE_STATE		m_state;
	bool				m_external;
	TextureDesc			m_desc;
};

//class CUTE_VK_API VK_Texture : public Texture
//{
//public:
//	VK_Texture(VK_Device* device, const TextureDesc& desc);
//	~VK_Texture();
//
//	void* map(PixelData& data, MAP_FLAG flag, uint level = 0, uint face = 0);
//	void  unmap();
//
//	VkImageView& getView() { return m_view; }
//
//private:
//	VK_Device*		m_device;
//	VkImage			m_image;
//	VkDeviceMemory	m_memory;
//	VkImageView		m_view;		//??
//};

CUTE_NS_END
