#include "VK_CommandBuffer.h"
#include "VK_Pipeline.h"
#include "VK_Buffer.h"

CU_NS_BEGIN

VK_CommandBuffer::VK_CommandBuffer()
{

}

VK_CommandBuffer::~VK_CommandBuffer()
{

}

void VK_CommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	VkViewport viewport = { (float)x, (float)y, (float)w, (float)h, 0.0f, 1.0f };
	vkCmdSetViewport(m_handle, 0, 1, &viewport);
}

void VK_CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{
	VkRect2D scissor = { x, y, w, h };
	vkCmdSetScissor(m_handle, 0, 1, &scissor);
}

void VK_CommandBuffer::setBlendFactor(const float factors[4])
{
	vkCmdSetBlendConstants(m_handle, factors);
}

void VK_CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	vkCmdSetStencilReference(m_handle, (VkStencilFaceFlags)mask, (uint32_t)reference);
}

void VK_CommandBuffer::setPipeline(Pipeline* pipeline)
{
	VK_Pipeline* ppl = pipeline->cast<VK_Pipeline>();
	vkCmdBindPipeline(m_handle, ppl->getBindPoint(), ppl->native());
}

void VK_CommandBuffer::setIndexBuffer(IndexBuffer* buffer)
{
	VK_Buffer* vk_buffer = buffer->cast<VK_Buffer>();
	vkCmdBindIndexBuffer(m_handle, vk_buffer->native(), 0, vk_buffer->getIndexType());
}

CU_NS_END
