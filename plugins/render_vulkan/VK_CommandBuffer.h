#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class CU_VK_API VKCommandBuffer : public CommandBuffer
{
public:
	VKCommandBuffer();
	~VKCommandBuffer();

	void setViewport(int x, int y, size_t w, size_t h);
	void setScissor(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);

	void setPipeline(Pipeline* pipeline);
	void setIndexBuffer(IndexBuffer* buffer);

private:
	VkCommandBuffer m_handle;
};

CU_NS_END
