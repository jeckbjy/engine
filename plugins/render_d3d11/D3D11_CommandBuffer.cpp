#include "D3D11_CommandBuffer.h"
#include "D3D11_Buffer.h"
#include "D3D11_Pipeline.h"
#include "D3D11_InputLayout.h"
#include "D3D11_DescriptorSet.h"

CU_NS_BEGIN

D3D11CommandBuffer::D3D11CommandBuffer()
{
	memset(m_factors, 0, sizeof(m_factors));
	m_stencilRef = 0;
}

D3D11CommandBuffer::~D3D11CommandBuffer()
{

}

void D3D11CommandBuffer::reset()
{

}

void D3D11CommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	D3D11_VIEWPORT viewport = { x, y, w, h, 0.0f, 1.0f };
	m_context->RSSetViewports(1, &viewport);
}

void D3D11CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{
	D3D11_RECT rect = { x, y, (int)x + w, (int)y + h};
	m_context->RSSetScissorRects(1, &rect);
}

void D3D11CommandBuffer::setBlendFactor(const float factors[4])
{
	memcpy(m_factors, factors, sizeof(m_factors));
}

void D3D11CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	m_stencilMask = mask;
	m_stencilRef = reference;
}

void D3D11CommandBuffer::setRenderTarget(RenderTarget* target)
{
	((D3D11RenderTarget*)target)->bind(m_context);
}

void D3D11CommandBuffer::setTopology(Topology primitive)
{
	D3D11_PRIMITIVE_TOPOLOGY dx_prim = (D3D11_PRIMITIVE_TOPOLOGY)primitive;
	m_context->IASetPrimitiveTopology(dx_prim);
}

void D3D11CommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{
	m_descriptors = descriptors->cast<D3D11DescriptorSet>();
}

void D3D11CommandBuffer::setPipeline(Pipeline* pipeline)
{
	m_pipeline = pipeline->cast<D3D11Pipeline>();
}

void D3D11CommandBuffer::setInputLayout(InputLayout* layout)
{
	m_layout = layout->cast<D3D11InputLayout>();
}

void D3D11CommandBuffer::setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets)
{
	ID3D11Buffer* dx_buffers[CU_MAX_BOUND_VERTEX_BUFFERS];
	UINT32 dx_strides[CU_MAX_BOUND_VERTEX_BUFFERS];
	UINT32 dx_offsets[CU_MAX_BOUND_VERTEX_BUFFERS];
	D3D11Buffer* buffer;
	for (size_t i = 0; i < counts; ++i)
	{
		buffer = buffers[i]->cast<D3D11Buffer>();
		dx_buffers[i] = buffer->native();
		dx_strides[i] = buffer->stride();
		dx_offsets[i] = offsets ? offsets[i] : 0;
	}

	m_context->IASetVertexBuffers(startSlot, counts, dx_buffers, dx_strides, dx_offsets);
}

void D3D11CommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{
	D3D11Buffer* dx_buffer = (D3D11Buffer*)buffer;
	DXGI_FORMAT format = buffer->isIndex16() ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_context->IASetIndexBuffer(dx_buffer->native(), format, offset);
}

void D3D11CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	prepare();
	if (instanceCount == 0)
	{
		m_context->Draw(vertexCount, vertexOffset);
	}
	else
	{
		m_context->DrawInstanced(vertexCount, instanceCount, vertexOffset, instanceOffset);
	}
}

void D3D11CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{
	prepare();
	if (instanceCount == 0)
	{
		m_context->DrawIndexed(indexCount, indexOffset, vertexOffset);
	}
	else
	{
		m_context->DrawIndexedInstanced(indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
	}
}

void D3D11CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{
	m_context->Dispatch(x, y, z);
}

void D3D11CommandBuffer::prepare()
{
	if (m_pipeline)
		m_pipeline->bind(this);
}

CU_NS_END
