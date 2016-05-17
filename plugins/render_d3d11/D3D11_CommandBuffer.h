#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Pipeline;
class D3D11InputLayout;
class D3D11CommandBuffer : public CommandBuffer
{
public:
	D3D11CommandBuffer();
	~D3D11CommandBuffer();

	void reset();
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);
	void setRenderTarget(RenderTarget* target);
	void setTopology(Topology primitive);
	void setPipeline(Pipeline* pipeline);
	void setInputLayout(InputLayout* layout);

	void setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets);
	void setIndexBuffer(IndexBuffer* buffer, size_t offset);

	void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
	void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
	void dispatch(size_t x, size_t y, size_t z);

	ID3D11ContextN*		getContext() { return m_context; }
	D3D11InputLayout*	getLayout() { return m_layout; }
	const float*		getFactors() const { return m_factors; }
	size_t				getStencilRef() const { return m_stencilRef; }
	UINT				getSampleMask() const{ return m_sampleMask; }

private:
	void prepare();

private:
	ID3D11ContextN*		m_context;
	D3D11Pipeline*		m_pipeline;
	D3D11InputLayout*	m_layout;
	float				m_factors[4];
	UINT				m_sampleMask;
	StencilFaceFlags	m_stencilMask;
	size_t				m_stencilRef;
};

CU_NS_END