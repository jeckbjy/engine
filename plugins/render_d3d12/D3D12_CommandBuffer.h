#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12CommandBuffer : public CommandBuffer
{
public:
	D3D12CommandBuffer(ID3D12Device* device, ID3D12CommandAllocator* alloc);
	~D3D12CommandBuffer();

	void setViewport(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);
	void setRenderTarget(RenderTarget* target);
	void setDescriptorSet(DescriptorSet* descriptors);
	void setPipeline(Pipeline* pipeline);
	void setVertexLayout(VertexLayout* vbs);
	void setIndexBuffer(IndexBuffer* ib);
	void draw(const DrawParam& params);
	void dispatch(size_t group_x, size_t group_y, size_t group_z);

	ID3D12GraphicsCommandList* native() { return m_handle; }

private:
	ID3D12GraphicsCommandList* m_handle;
};

CU_NS_END
