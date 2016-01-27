#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11CommandBuffer : public CommandBuffer
{
public:
	D3D11CommandBuffer();
	~D3D11CommandBuffer();

	void setBlendFactor(const float factors[4]);
	void setStencilRef(size_t stencil);
	void setRenderTarget(RenderTarget* target);
	void setTopology(Topology primitive);
	void setPipeline(Pipeline* pipeline);
	void setVertexLayout(VertexLayout* vbs);
	void setIndexBuffer(IndexBuffer* ib);
	void draw(size_t vnum, size_t voff /* = 0 */, size_t instance_num /* = 1 */, size_t instance_off /* = 0 */);
	void drawIndexed(size_t inum, size_t ioff /* = 0 */, size_t instance_num /* = 1 */, size_t instance_off /* = 0 */, int vertex_base /* = 0 */);
	void dispatch(size_t group_x, size_t group_y, size_t group_z);

private:
	ID3D11DeviceContextN* _handle;
	float _factors[4];
	size_t _stencilref;
};

CU_NS_END