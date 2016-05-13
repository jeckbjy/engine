#include "D3D12_CommandBuffer.h"

CU_NS_BEGIN

D3D12CommandBuffer::D3D12CommandBuffer(ID3D12Device* device, ID3D12CommandAllocator* alloc)
{
	D3D12_CHECK(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc, 0, IID_PPV_ARGS(&m_handle))
		,"D3D12 CreateCommandList Fail!");
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

}

void D3D12CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{

}

void D3D12CommandBuffer::setBlendFactor(const float factors[4])
{

}

void D3D12CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{

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

}

void D3D12CommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{

}

void D3D12CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{

}

void D3D12CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{

}

void D3D12CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{

}

CU_NS_END
