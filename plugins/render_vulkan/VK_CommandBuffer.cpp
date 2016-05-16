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
	//VK_Pipeline* ppl = pipeline->cast<VK_Pipeline>();
	//vkCmdBindPipeline(m_handle, ppl->getBindPoint(), ppl->native());
}

void VK_CommandBuffer::setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets)
{
	VkBuffer vk_buffers[CU_MAX_BOUND_VERTEX_BUFFERS];
	VkDeviceSize vk_offsets[CU_MAX_BOUND_VERTEX_BUFFERS];

	for (size_t i = 0; i < counts; ++i)
	{
		VK_Buffer* tmp = (VK_Buffer*)buffers[i];
		vk_buffers[i] = tmp->native();
		vk_offsets[i] = offsets ? offsets[i] : 0;
	}

	vkCmdBindVertexBuffers(m_handle, startSlot, counts, vk_buffers ,vk_offsets);
}

void VK_CommandBuffer::setIndexBuffer(IndexBuffer* buffer)
{
	VK_Buffer* vk_buffer = buffer->cast<VK_Buffer>();
	vkCmdBindIndexBuffer(m_handle, vk_buffer->native(), 0, vk_buffer->getIndexType());
}

void VK_CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	vkCmdDraw(m_handle, vertexCount, instanceCount, vertexOffset, instanceOffset);
}

void VK_CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{
	vkCmdDrawIndexed(m_handle, indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
}

void VK_CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{
	vkCmdDispatch(m_handle, x, y, z);
}

CU_NS_END
