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

void D3D12CommandBuffer::setIndexBuffer(IndexBuffer* ib)
{

}

void D3D12CommandBuffer::draw(const DrawParam& params)
{

}

void D3D12CommandBuffer::dispatch(size_t group_x, size_t group_y, size_t group_z)
{

}

CU_NS_END
