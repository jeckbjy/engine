#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_Device : public Device
{
public:
	D3D12_Device();
	~D3D12_Device();

	GpuBuffer*		newBuffer(const BufferDesc& desc);
	Texture*		newTexture(const TextureDesc& desc);
	//RenderTarget*	newRenderWindow(Window* hwnd);
	//RenderTarget*	newRenderTexture(Texture* rtv, Texture* dsv /* = NULL */);
	InputLayout*	newInputLayout(const InputElement* elements, size_t count);
	ShaderStage*	newProgram();
	Pipeline*		newPipeline(const PipelineDesc& desc);
	DescriptorSet*	newDescriptorSet(ShaderStage* prog);
	CommandBuffer*	newCommandBuffer();
	CommandQueue*	newCommandQueue();
	FrameBuffer*	newFrameBuffer();
	SwapChain*		newSwapChain();

	ID3D12Device* getDevice() { return m_device; }
	ID3D12CommandAllocator* getAllocator() { return m_allocator; }

	operator ID3D12Device*() const { return m_device; }

private:
	ID3D12Device*			m_device;
	ID3D12CommandAllocator* m_allocator;
};

extern CU_D3D12_API D3D12_Device*	gD3D12Device();
extern CU_D3D12_API ID3D12Device*	gD3D12NativeDevice();

CU_NS_END
