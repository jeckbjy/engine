#include "VK_CommandList.h"
//#include "VK_Buffer.h"
//#include "VK_Pipeline.h"
//#include "VK_VertexLayout.h"

CUTE_NS_BEGIN

VK_CommandList::VK_CommandList()
{

}

VK_CommandList::~VK_CommandList()
{

}

bool VK_CommandList::init(VK_Device* device, COMMANDLIST_TYPE type)
{
	return true;
}

void VK_CommandList::term()
{
}

void VK_CommandList::setBlendConstant(const float values[4])
{
	vkCmdSetBlendConstants(m_buffer, values);
}

void VK_CommandList::setStencilReference(uint32 stencilRef)
{
	vkCmdSetStencilReference(m_buffer, VK_STENCIL_FRONT_AND_BACK, stencilRef);
}

void VK_CommandList::setViewports(uint32 count, Viewport* viewports)
{
	assert(sizeof(Viewport) == sizeof(VkViewport));

	if (count == 0)
		return;

	VkViewport* pNativeViewports = reinterpret_cast<VkViewport*>(viewports);
	vkCmdSetViewport(m_buffer, 0, count, pNativeViewports);
}

void VK_CommandList::setScissors(uint32 count, Rect* scissors)
{
	assert(sizeof(Rect) == sizeof(VkViewport));

	if (count == 0)
		return;

	VkRect2D* pNativeScissors = reinterpret_cast<VkRect2D*>(scissors);

	vkCmdSetScissor(m_buffer, 0, count, pNativeScissors);
}


//void VK_CommandList::setRenderTarget(RenderTarget* target)
//{
//	target->bind(NULL);
//}
//
//void VK_CommandList::setViewport(int x, int y, size_t w, size_t h)
//{
//	VkViewport viewport = { (float)x, (float)y, (float)w, (float)h, 0.0f, 1.0f };
//	vkCmdSetViewport(m_handle, 0, 1, &viewport);
//}
//
//void VK_CommandList::setScissor(int x, int y, size_t w, size_t h)
//{
//	VkRect2D scissor = { x, y, w, h };
//	vkCmdSetScissor(m_handle, 0, 1, &scissor);
//}
//
//void VK_CommandList::setBlendFactor(const float factors[4])
//{
//	vkCmdSetBlendConstants(m_handle, factors);
//}
//
//void VK_CommandList::setStencilRef(StencilFaceFlags mask, size_t reference)
//{
//	vkCmdSetStencilReference(m_handle, (VkStencilFaceFlags)mask, (uint32_t)reference);
//}
//
//void VK_CommandList::setTopology(Topology topology)
//{
//
//}
//
//void VK_CommandList::setDescriptorSet(DescriptorSet* descriptors)
//{
//
//}
//
//void VK_CommandList::setPipeline(Pipeline* pipeline)
//{
//	m_pipeline = pipeline->cast<VK_Pipeline>();
//}
//
////void VK_CommandBuffer::setInputLayout(VertexLayout* layout)
////{
////	m_layout = layout->cast<VK_InputLayout>();
////}
////
////void VK_CommandBuffer::setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets)
////{
////	VkBuffer vk_buffers[CU_MAX_VERTEX_BUFFERS];
////	VkDeviceSize vk_offsets[CU_MAX_VERTEX_BUFFERS];
////
////	for (size_t i = 0; i < counts; ++i)
////	{
////		VK_Buffer* tmp = (VK_Buffer*)buffers[i];
////		vk_buffers[i] = tmp->native();
////		vk_offsets[i] = offsets ? offsets[i] : 0;
////	}
////
////	vkCmdBindVertexBuffers(m_handle, startSlot, counts, vk_buffers ,vk_offsets);
////}
//
//void VK_CommandList::setVertexArray(VertexArray* vertexs)
//{
//	m_layout = vertexs->getLayout()->cast<VK_VertexLayout>();
//
//	VkBuffer vk_buffers[CU_MAX_VERTEX_BUFFERS];
//	VkDeviceSize vk_offsets[CU_MAX_VERTEX_BUFFERS];
//
//	size_t counts = vertexs->getBufferCount();
//	size_t startSlot = vertexs->getStartSlot();
//
//	for (size_t i = 0; i < counts; ++i)
//	{
//		VK_Buffer* tmp = (VK_Buffer*)vertexs->getBuffer(i);
//		vk_buffers[i] = tmp->native();
//		vk_offsets[i] = vertexs->getOffset(i);
//	}
//
//	vkCmdBindVertexBuffers(m_handle, startSlot, counts, vk_buffers, vk_offsets);
//}
//
//void VK_CommandList::setIndexBuffer(IndexBuffer* buffer, size_t offset)
//{
//	VK_Buffer* vk_buffer = buffer->cast<VK_Buffer>();
//	vkCmdBindIndexBuffer(m_handle, vk_buffer->native(), 0, vk_buffer->getIndexType());
//}
//
//void VK_CommandList::clear(ClearMask masks, const Color& color, float depth, uint32_t stencil, uint8_t targetMask)
//{
//
//}
//
//void VK_CommandList::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
//{
//	prepare();
//	vkCmdDraw(m_handle, vertexCount, instanceCount, vertexOffset, instanceOffset);
//}
//
//void VK_CommandList::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
//{
//	prepare();
//	vkCmdDrawIndexed(m_handle, indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
//}
//
//void VK_CommandList::dispatch(size_t x, size_t y, size_t z)
//{
//	vkCmdDispatch(m_handle, x, y, z);
//}

//void VK_CommandList::prepare()
//{
//	if (m_pipeline)
//		m_pipeline->bind(this);
//}

CUTE_NS_END
