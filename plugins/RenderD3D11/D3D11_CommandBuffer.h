#pragma once
#include "D3D11_API.h"

CUTE_NS_BEGIN

class D3D11_Pipeline;
class D3D11_VertexLayout;
class D3D11_DescriptorSet;

class D3D11_CommandBuffer : public CommandBuffer
{
public:
	D3D11_CommandBuffer(ID3D11DeviceN* device, ID3D11ContextN* context);
	~D3D11_CommandBuffer();

	void setRenderTarget(RenderTarget* target);
	void setViewport(int x, int y, size_t w, size_t h);
	void setScissor(int x, int y, size_t w, size_t h);

	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);
	void setTopology(Topology topology);
	void setDescriptorSet(DescriptorSet* descriptors);
	void setPipeline(Pipeline* pipeline);
	void setVertexArray(VertexArray* vertexs);
	void setIndexBuffer(IndexBuffer* buffer, size_t offset);

	void clear(ClearMask masks /* = CLEAR_ALL */, const Color& color /* = Color::Black */, float depth /* = 1.0f */, uint32_t stencil /* = 0 */, uint8_t targetMask);
	void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
	void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
	void dispatch(size_t x, size_t y, size_t z);

	ID3D11ContextN*		getContext() { return m_context; }
	D3D11_VertexLayout*	getLayout() { return m_layout; }
	const float*		getFactors() const { return m_factors; }
	size_t				getStencilRef() const { return m_stencilRef; }
	UINT				getSampleMask() const{ return m_sampleMask; }

private:
	void prepare();

private:
	ID3D11ContextN*			m_context;
	RenderTarget*			m_target;
	D3D11_Pipeline*			m_pipeline;
	D3D11_VertexLayout*		m_layout;
	D3D11_DescriptorSet*	m_descriptors;
	float					m_factors[4];
	UINT					m_sampleMask;
	StencilFaceFlags		m_stencilMask;
	size_t					m_stencilRef;
};

CUTE_NS_END
