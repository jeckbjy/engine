#include "D3D12_Device.h"
#include "D3D12_Buffer.h"
#include "D3D12_Texture.h"
#include "D3D12_CommandBuffer.h"
#include "D3D12_CommandQueue.h"

CU_NS_BEGIN

D3D12Device::D3D12Device()
:m_device(NULL)
{
	D3D12_CHECK(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)), 
		"d3d12 create ");
}

D3D12Device::~D3D12Device()
{
	D3D12_RELEASE(m_device);
}

GpuBuffer* D3D12Device::newBuffer(const BUFFER_DESC& desc)
{
	return new D3D12Buffer(desc);
}

Texture* D3D12Device::newTexture(const TEXTURE_DESC& desc)
{
	return new D3D12Texture(desc);
}

RenderTarget* D3D12Device::newRenderWindow(Window* hwnd)
{
	return NULL;
}

RenderTarget* D3D12Device::newRenderTexture(Texture* rtv, Texture* dsv)
{
	return NULL;
}

Program* D3D12Device::newProgram()
{
	return NULL;
}

Pipeline* D3D12Device::newPipeline(const PIPELINE_DESC& desc)
{
	return NULL;
}

DescriptorSet* D3D12Device::newDescriptorSet(Program* prog)
{
	return NULL;
}

CommandBuffer* D3D12Device::newCommandBuffer()
{
	return new D3D12CommandBuffer(m_device, m_alloc);
}

CommandQueue* D3D12Device::newCommandQueue()
{
	return new D3D12CommondQueue(m_device);
}

CU_NS_END