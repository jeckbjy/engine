#include "D3D11_CommandBuffer.h"
#include "D3D11_Pipeline.h"
#include "D3D11_VertexLayout.h"

CU_NS_BEGIN

D3D11CommandBuffer::D3D11CommandBuffer()
{
	memset(_factors, 0, sizeof(_factors));
	_stencilref = 0;
}

D3D11CommandBuffer::~D3D11CommandBuffer()
{

}

void D3D11CommandBuffer::setBlendFactor(const float factors[4])
{
	memcpy(_factors, factors, sizeof(_factors));
}

void D3D11CommandBuffer::setStencilRef(size_t stencil)
{
	_stencilref = stencil;
}

void D3D11CommandBuffer::setRenderTarget(RenderTarget* target)
{
	((D3D11RenderTarget*)target)->bind(_handle);
}

void D3D11CommandBuffer::setTopology(Topology primitive)
{
	D3D11_PRIMITIVE_TOPOLOGY dx_prim = (D3D11_PRIMITIVE_TOPOLOGY)primitive;
	_handle->IASetPrimitiveTopology(dx_prim);
}

void D3D11CommandBuffer::setPipeline(Pipeline* pipeline)
{
	((D3D11Pipeline*)pipeline)->bind(_handle, _factors, _stencilref);
}

void D3D11CommandBuffer::setVertexLayout(VertexLayout* vbs)
{
	// bind
	// ≤È’“…Ë÷√£ø£ø
}

void D3D11CommandBuffer::setIndexBuffer(IndexBuffer* ib)
{

}

void D3D11CommandBuffer::draw(size_t vnum, size_t voff /* = 0 */, size_t instance_num /* = 1 */, size_t instance_off /* = 0 */)
{

}

void D3D11CommandBuffer::drawIndexed(size_t inum, size_t ioff, size_t instance_num, size_t instance_off, int vertex_base)
{

}

void D3D11CommandBuffer::dispatch(size_t group_x, size_t group_y, size_t group_z)
{

}

CU_NS_END