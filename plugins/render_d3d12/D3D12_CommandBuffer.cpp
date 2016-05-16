#include "D3D12_CommandBuffer.h"
#include "D3D12_Buffer.h"

CU_NS_BEGIN

D3D12CommandBuffer::D3D12CommandBuffer(ID3D12Device* device, ID3D12CommandAllocator* alloc)
{
	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc, 0, IID_PPV_ARGS(&m_handle));
	D3D12_CHECK(hr, "D3D12 CreateCommandList Fail!");
}

D3D12CommandBuffer::~D3D12CommandBuffer()
{
	D3D12_RELEASE(m_handle);
}

void D3D12CommandBuffer::reset()
{
	m_handle->Reset(NULL, NULL);
}

void D3D12CommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	// 支持多个?
	//m_handle->RSSetViewports();
}

void D3D12CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{
	//m_handle->RSSetScissorRects()
}

void D3D12CommandBuffer::setBlendFactor(const float factors[4])
{
	m_handle->OMSetBlendFactor(factors);
}

void D3D12CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	m_handle->OMSetStencilRef(ref);
}

void D3D12CommandBuffer::setRenderTarget(RenderTarget* target)
{

}

void D3D12CommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{

}

void D3D12CommandBuffer::setPipeline(Pipeline* pipeline)
{

}

void D3D12CommandBuffer::setInputLayout(InputLayout* layout)
{

}

void D3D12CommandBuffer::setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets)
{
	D3D12_VERTEX_BUFFER_VIEW views[CU_MAX_BOUND_VERTEX_BUFFERS];
	for (size_t i = 0; i < counts; ++i)
	{
		D3D12Buffer* dx_buffer = (D3D12Buffer*)buffers[i];
		views[i].BufferLocation = dx_buffer->getGPUVirtualAddress();
		views[i].SizeInBytes = dx_buffer->bytes();
		views[i].StrideInBytes = dx_buffer->stride();
	}
	m_handle->IASetVertexBuffers(startSlot, counts, views);
}

void D3D12CommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{
	D3D12Buffer* dx_buffer = (D3D12Buffer*)buffer;
	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = dx_buffer->getGPUVirtualAddress() + offset;
	view.SizeInBytes = dx_buffer->bytes() - offset;
	view.Format = buffer->isIndex16() ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_handle->IASetIndexBuffer(&view);
}

void D3D12CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	m_handle->DrawInstanced(vertexCount, instanceCount, vertexOffset, instanceOffset);
}

void D3D12CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{
	m_handle->DrawIndexedInstanced(indexCount, instanceCount, indexOffset, vertexOffset, instanceCount);
}

void D3D12CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{
	m_handle->Dispatch(x, y, z);
}

CU_NS_END
