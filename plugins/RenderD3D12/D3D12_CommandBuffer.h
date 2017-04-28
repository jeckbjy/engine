#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class D3D12_Pipeline;
class D3D12_VertexLayout;
class CUTE_D3D12_API D3D12_CommandBuffer : public CommandBuffer
{
public:
	D3D12_CommandBuffer(ID3D12Device* device, ID3D12CommandAllocator* alloc);
	~D3D12_CommandBuffer();

	void setRenderTarget(RenderTarget* target);
	void setViewport(int x, int y, size_t w, size_t h);
	void setScissor(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t stencilRef);
	void setTopology(Topology topology);
	void setDescriptorSet(DescriptorSet* descriptors);
	void setPipeline(Pipeline* pipeline);
	void setVertexArray(VertexArray* vertexs);
	void setIndexBuffer(IndexBuffer* buffer, size_t offset);

	void clear(ClearMask masks /* = CLEAR_ALL */, const Color& color /* = Color::BLACK */, float depth /* = 1.0f */, uint32_t stencil /* = 0 */, uint8_t targetMask /* = 0xFF */);
	void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
	void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
	void dispatch(size_t x, size_t y, size_t z);

	ID3D12GraphicsCommandList* native() { return m_handle; }

	D3D12_VertexLayout* getLayout() { return m_layout; }

private:
	void prepare();

private:
	ID3D12GraphicsCommandList*	m_handle;
	D3D12_VertexLayout*			m_layout;
	D3D12_Pipeline*				m_pipeline;
};

CUTE_NS_END
