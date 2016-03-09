#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKTexture : public Texture
{
public:
	VKTexture(const TEXTURE_DESC& desc);
	~VKTexture();

private:
	VkImage			m_image;
	VkDeviceMemory	m_memory;
	VkImageView		m_view;		//??
};

CU_NS_END
