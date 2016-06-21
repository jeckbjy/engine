#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class VK_Pipeline;
class VK_InputLayout;
class CU_VK_API VK_CommandBuffer : public CommandBuffer
{
public:
	VK_CommandBuffer();
	~VK_CommandBuffer();

	void setRenderTarget(RenderTarget* target);
	void setViewport(int x, int y, size_t w, size_t h);
	void setScissor(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);

	void setDescriptorSet(DescriptorSet* descriptors);
	void setPipeline(Pipeline* pipeline);
	void setInputLayout(InputLayout* layout);
	void setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets);
	void setIndexBuffer(IndexBuffer* buffer, size_t offset);

	void clear(ClearMask masks /* = CLEAR_ALL */, const Color& color /* = Color::BLACK */, float depth /* = 1.0f */, uint32_t stencil /* = 0 */, uint8_t targetMask /* = 0xFF */);
	void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
	void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
	void dispatch(size_t x, size_t y, size_t z);

	VkCommandBuffer	getNative() { return m_handle; }
	VK_InputLayout* getLayout() { return m_layout; }

private:
	void prepare();

private:
	VkCommandBuffer m_handle;
	VK_InputLayout*	m_layout;
	VK_Pipeline*	m_pipeline;
};

CU_NS_END
