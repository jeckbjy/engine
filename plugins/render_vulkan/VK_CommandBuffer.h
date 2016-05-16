#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VK_CommandBuffer : public CommandBuffer
{
public:
	VK_CommandBuffer();
	~VK_CommandBuffer();

	void setViewport(int x, int y, size_t w, size_t h);
	void setScissor(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);

	void setPipeline(Pipeline* pipeline);

	void setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets);
	void setIndexBuffer(IndexBuffer* buffer);

	void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
	void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
	void dispatch(size_t x, size_t y, size_t z);

private:
	VkCommandBuffer m_handle;
};

CU_NS_END
