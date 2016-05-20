#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class VK_Device;
class CU_VK_API VK_Texture : public Texture
{
public:
	VK_Texture(VK_Device* device, const TextureDesc& desc);
	~VK_Texture();

private:
	VK_Device*		m_device;
	VkImage			m_image;
	VkDeviceMemory	m_memory;
	VkImageView		m_view;		//??
};

CU_NS_END
