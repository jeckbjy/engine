#include "VK_FrameBuffer.h"
#include "VK_Texture.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

VK_FrameBuffer::VK_FrameBuffer()
	: m_device(NULL)
	, m_buffer(VK_NULL_HANDLE)
	, m_pass(VK_NULL_HANDLE)
{
}

VK_FrameBuffer::~VK_FrameBuffer()
{
	term();
}

bool VK_FrameBuffer::init(VK_Device* device, const FrameBufferDesc& desc)
{
	m_device = device;

	VkAttachmentDescription attachmentDesc[9] = {};
    VkAttachmentReference   attachmentRefs[9] = {};
    VkImageView             imageViews    [9] = {};
    VkAttachmentReference*  pDepthAttachmentRef = NULL;

	uint32_t attachmentCount = desc.colorCount;
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t layers = 0;

	for (size_t i = 0; i < desc.colorCount; ++i)
	{
		VK_Texture* wrapTexture = static_cast<VK_TextureView>(desc.colorTargets[i]);
	}
	return true;
}

void VK_FrameBuffer::term()
{

}

CUTE_NS_END
