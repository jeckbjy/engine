#include "D3D12_Device.h"
#include "D3D12_Buffer.h"
#include "D3D12_Texture.h"
#include "D3D12_CommandBuffer.h"
#include "D3D12_CommandQueue.h"
#include "D3D12_SwapChain.h"
#include "D3D12_FrameBuffer.h"
#include "Engine.h"

CU_NS_BEGIN

D3D12_Device*	gD3D12Device()
{
	return gGraphics.getDevice()->cast<D3D12_Device>();
}

ID3D12Device*	gD3D11NativeDevice()
{
	D3D12_Device* device = gGraphics.getDevice()->cast<D3D12_Device>();
	if (device)
		return device->getDevice();

	return NULL;
}

D3D12_Device::D3D12_Device()
:m_device(NULL)
{
	D3D12_CHECK(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)), "D3D12CreateDevice fail!");
}

D3D12_Device::~D3D12_Device()
{
	D3D12_RELEASE(m_device);
}

GpuBuffer* D3D12_Device::newBuffer(const BufferDesc& desc)
{
	return new D3D12_Buffer(m_device, desc);
}

Texture* D3D12_Device::newTexture(const TextureDesc& desc)
{
	return new D3D12_Texture(m_device, desc);
}

//RenderTarget* D3D12_Device::newRenderWindow(Window* hwnd)
//{
//	return NULL;
//}
//
//RenderTarget* D3D12_Device::newRenderTexture(Texture* rtv, Texture* dsv)
//{
//	return NULL;
//}

InputLayout* D3D12_Device::newInputLayout(const InputElement* elements, size_t count)
{
	return NULL;
}

ShaderStage* D3D12_Device::newProgram()
{
	return NULL;
}

Pipeline* D3D12_Device::newPipeline(const PipelineDesc& desc)
{
	return NULL;
}

DescriptorSet* D3D12_Device::newDescriptorSet(ShaderStage* prog)
{
	return NULL;
}

CommandBuffer* D3D12_Device::newCommandBuffer()
{
	return new D3D12_CommandBuffer(m_device, m_allocator);
}

CommandQueue* D3D12_Device::newCommandQueue()
{
	return new D3D12_CommondQueue(m_device);
}

FrameBuffer* D3D12_Device::newFrameBuffer()
{
	return new D3D12_FrameBuffer();
}

SwapChain* D3D12_Device::newSwapChain()
{
	return new D3D12_SwapChain();
}

CU_NS_END