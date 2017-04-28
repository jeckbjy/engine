#include "D3D12_CommandBuffer.h"
#include "D3D12_VertexLayout.h"
#include "D3D12_Pipeline.h"
#include "D3D12_Buffer.h"

CUTE_NS_BEGIN

D3D12_CommandBuffer::D3D12_CommandBuffer(ID3D12Device* device, ID3D12CommandAllocator* alloc)
{
	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc, 0, IID_PPV_ARGS(&m_handle));
	D3D12_CHECK(hr, "D3D12 CreateCommandList Fail!");
}

D3D12_CommandBuffer::~D3D12_CommandBuffer()
{
	D3D12_RELEASE(m_handle);
}

void D3D12_CommandBuffer::setRenderTarget(RenderTarget* target)
{

}

void D3D12_CommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	D3D12_VIEWPORT viewport = { x, y, w, h, 0.0f, 1.0f };
	m_handle->RSSetViewports(1, &viewport);
}

void D3D12_CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{
	D3D12_RECT rect = { x, y, (int)x + w, (int)y + h };
	m_handle->RSSetScissorRects(1, &rect);
}

void D3D12_CommandBuffer::setBlendFactor(const float factors[4])
{
	m_handle->OMSetBlendFactor(factors);
}

void D3D12_CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t stencilRef)
{
	m_handle->OMSetStencilRef(stencilRef);
}

void D3D12_CommandBuffer::setTopology(Topology topology)
{
	D3D12_PRIMITIVE_TOPOLOGY primite = (D3D12_PRIMITIVE_TOPOLOGY)topology;
	m_handle->IASetPrimitiveTopology(primite);
}

void D3D12_CommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{

}

void D3D12_CommandBuffer::setPipeline(Pipeline* pipeline)
{
	m_pipeline = (D3D12_Pipeline*)pipeline;
}

void D3D12_CommandBuffer::setVertexArray(VertexArray* vertexs)
{
	m_layout = (D3D12_VertexLayout*)vertexs->getLayout();

	D3D12_VERTEX_BUFFER_VIEW views[CU_MAX_VERTEX_BUFFERS];

	size_t counts = vertexs->getBufferCount();
	size_t start = vertexs->getStartSlot();
	for (size_t i = 0; i < counts; ++i)
	{
		D3D12_Buffer* buffer = (D3D12_Buffer*)vertexs->getBuffer(i);
		views[i].BufferLocation = buffer->getGPUVirtualAddress();
		views[i].SizeInBytes = buffer->bytes();
		views[i].StrideInBytes = buffer->stride();
	}
	m_handle->IASetVertexBuffers(start, counts, views);
}

void D3D12_CommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{
	D3D12_Buffer* dx_buffer = (D3D12_Buffer*)buffer;
	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = dx_buffer->getGPUVirtualAddress() + offset;
	view.SizeInBytes = dx_buffer->bytes() - offset;
	view.Format = buffer->isIndex16() ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_handle->IASetIndexBuffer(&view);
}

void D3D12_CommandBuffer::clear(ClearMask masks, const Color& color, float depth, uint32_t stencil, uint8_t targetMask)
{

}

void D3D12_CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	prepare();
	m_handle->DrawInstanced(vertexCount, instanceCount, vertexOffset, instanceOffset);
}

void D3D12_CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{
	prepare();
	m_handle->DrawIndexedInstanced(indexCount, instanceCount, indexOffset, vertexOffset, instanceCount);
}

void D3D12_CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{
	m_handle->Dispatch(x, y, z);
}

void D3D12_CommandBuffer::prepare()
{
	if (m_pipeline)
		m_pipeline->bind(this);
}

CUTE_NS_END
