#include "VK_CommandBuffer.h"
#include "VK_Pipeline.h"
#include "VK_Buffer.h"

CU_NS_BEGIN

VKCommandBuffer::VKCommandBuffer()
{

}

VKCommandBuffer::~VKCommandBuffer()
{

}

void VKCommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	VkViewport viewport = { (float)x, (float)y, (float)w, (float)h, 0.0f, 1.0f };
	vkCmdSetViewport(m_handle, 0, 1, &viewport);
}

void VKCommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{
	VkRect2D scissor = { x, y, w, h };
	vkCmdSetScissor(m_handle, 0, 1, &scissor);
}

void VKCommandBuffer::setBlendFactor(const float factors[4])
{
	vkCmdSetBlendConstants(m_handle, factors);
}

void VKCommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	vkCmdSetStencilReference(m_handle, (VkStencilFaceFlags)mask, (uint32_t)reference);
}

void VKCommandBuffer::setPipeline(Pipeline* pipeline)
{
	VKPipeline* ppl = pipeline->cast<VKPipeline>();
	vkCmdBindPipeline(m_handle, ppl->getBindPoint(), ppl->native());
}

void VKCommandBuffer::setIndexBuffer(IndexBuffer* buffer)
{
	VKBuffer* vk_buffer = buffer->cast<VKBuffer>();
	vkCmdBindIndexBuffer(m_handle, vk_buffer->native(), 0, vk_buffer->getIndexType());
}

CU_NS_END
