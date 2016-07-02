#include "VK_FrameBuffer.h"
#include "VK_Texture.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_FrameBuffer::VK_FrameBuffer()
: m_fbo(VK_NULL_HANDLE)
{

}

VK_FrameBuffer::~VK_FrameBuffer()
{
	destroy();
}

void VK_FrameBuffer::destroy()
{
	if (m_fbo != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(gVKNativeDevice(), m_fbo, NULL);
		m_fbo = VK_NULL_HANDLE;
	}
}

void VK_FrameBuffer::bind(void*)
{

}

void VK_FrameBuffer::update()
{
	m_dirty = false;

	VK_Texture* tex;
	VkImageView attachments[9] = { 0 };
	for (size_t i = 0; i < m_attachments.size(); ++i)
	{
		tex = (VK_Texture*)m_attachments[i].get();
		attachments[i + 1] = tex ? tex->getView() : VK_NULL_HANDLE;
	}

	VkFramebufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.pNext = NULL;
	info.renderPass = VK_NULL_HANDLE;	// todo:create by framebuffer
	info.pAttachments = attachments;
	info.attachmentCount = m_attachments.size();
	info.width = 0;
	info.height = 0;
	info.layers = 1;

	vkCreateFramebuffer(gVKNativeDevice(), &info, NULL, &m_fbo);
}

CU_NS_END