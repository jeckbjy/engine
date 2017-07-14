#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class VK_Texture;
class CUTE_VK_API VK_TextureView : public ITextureView
{
public:
	VK_TextureView();
	~VK_TextureView();

	bool init(VK_Device* device, ITexture* tex, const TextureViewDesc& desc);
	void term();

private:
	VK_Device*			m_device;
	VK_Texture*			m_texture;
	VkImageView			m_view;
	VkImageAspectFlags	m_aspect;
};

CUTE_NS_END
