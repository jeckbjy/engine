#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Device : public Device
{
public:
	D3D12Device();
	~D3D12Device();

	GpuBuffer*		newBuffer(const BUFFER_DESC& desc);
	Texture*		newTexture(const TEXTURE_DESC& desc);
	RenderTarget*	newRenderWindow(Window* hwnd);
	RenderTarget*	newRenderTexture(Texture* rtv, Texture* dsv /* = NULL */);
	VertexLayout*	newVertexLayout(VertexDeclaration& desc);
	Program*		newProgram();
	Pipeline*		newPipeline(const PIPELINE_DESC& desc);
	DescriptorSet*	newDescriptorSet(Program* prog);
	CommandBuffer*	newCommandBuffer();
	CommandQueue*	newCommandQueue();

	ID3D12Device* getDevice() { return m_device; }
	ID3D12CommandAllocator* getAlloc() { return m_alloc; }

private:
	ID3D12Device*			m_device;
	ID3D12CommandAllocator* m_alloc;
};

CU_NS_END
