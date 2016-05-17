#include "D3D12_Device.h"
#include "D3D12_Buffer.h"
#include "D3D12_Texture.h"
#include "D3D12_CommandBuffer.h"
#include "D3D12_CommandQueue.h"
#include "Engine.h"

CU_NS_BEGIN

D3D12Device::D3D12Device()
:m_device(NULL)
{
	D3D12_CHECK(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)), "D3D12CreateDevice fail!");
}

D3D12Device::~D3D12Device()
{
	D3D12_RELEASE(m_device);
}

GpuBuffer* D3D12Device::newBuffer(const BufferDesc& desc)
{
	return new D3D12Buffer(m_device, desc);
}

Texture* D3D12Device::newTexture(const TextureDesc& desc)
{
	return new D3D12Texture(m_device, desc);
}

RenderTarget* D3D12Device::newRenderWindow(Window* hwnd)
{
	return NULL;
}

RenderTarget* D3D12Device::newRenderTexture(Texture* rtv, Texture* dsv)
{
	return NULL;
}

InputLayout* D3D12Device::newInputLayout(const InputElement* elements, size_t count)
{
	return NULL;
}

Program* D3D12Device::newProgram()
{
	return NULL;
}

Pipeline* D3D12Device::newPipeline(const GraphicsPipelineDesc& desc)
{
	return NULL;
}

Pipeline* D3D12Device::newPipeline(const ComputePipelineDesc* desc)
{
	return NULL;
}

DescriptorSet* D3D12Device::newDescriptorSet(Program* prog)
{
	return NULL;
}

CommandBuffer* D3D12Device::newCommandBuffer()
{
	return new D3D12CommandBuffer(m_device, m_allocator);
}

CommandQueue* D3D12Device::newCommandQueue()
{
	return new D3D12CommondQueue(m_device);
}

D3D12Device*	gD3D12Device()
{
	return gEngine.getDevice()->cast<D3D12Device>();
}

ID3D12Device*	gD3D11NativeDevice()
{
	D3D12Device* device = gEngine.getDevice()->cast<D3D12Device>();
	if (device)
		return device->getDevice();

	return NULL;
}

CU_NS_END