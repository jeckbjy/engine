#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class OGL_Shader;
class OGL_Pipeline;
class OGL_InputLayout;
class OGL_Buffer;
class CU_OGL_API OGL_CommandBuffer : public CommandBuffer
{
public:
	OGL_CommandBuffer();
	~OGL_CommandBuffer();

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

private:
	void prepare();

private:
	RenderTarget*		m_target;
	OGL_Pipeline*		m_pipeline;
	OGL_InputLayout*	m_layout;
	OGL_Buffer*			m_index;
	DescriptorSet*		m_descriptors;
	Topology			m_primitive;
	size_t				m_vertexStart;
	size_t				m_vertexCount;	// ¶¥µãÊýÄ¿
	size_t				m_indexStart;
	size_t				m_indexCount;
	size_t				m_instanceCount;

	float				m_factors[4];
	StencilFaceFlags	m_stencilMask;
	size_t				m_stencilRef;
};

class CU_OGL_API OGLCommandQueue : public CommandQueue
{
public:
	void submit(CommandBuffer* cmds, Fence* fence){}
	void waitIdle(){}
	//void execute(CommandBuffer* cmd){ ((OGLCommandBuffer*)cmd)->execute();  }
};

CU_NS_END