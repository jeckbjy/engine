#include "D3D12_Device.h"
#include "D3D12_DescriptorHeap.h"

#include "D3D12_Buffer.h"
#include "D3D12_Texture.h"
//#include "D3D12_CommandBuffer.h"
//#include "D3D12_CommandQueue.h"
//#include "D3D12_SwapChain.h"
//#include "D3D12_FrameBuffer.h"
//#include "Engine.h"

CUTE_NS_BEGIN

D3D12_Device::D3D12_Device()
	: m_factory(NULL)
	, m_adapter(NULL)
	, m_output(NULL)
	, m_device(NULL)
{
}

D3D12_Device::~D3D12_Device()
{
	term();
}

bool D3D12_Device::init(const DeviceDesc& desc)
{
	HRESULT hr;

	uint32_t flags = 0;
	if (desc.enableDebug)
	{
		ID3D12Debug* pDebug = NULL;
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug));
		if (SUCCEEDED(hr))
		{
			pDebug->EnableDebugLayer();
		}

		D3D12_RELEASE(pDebug);
		flags |= DXGI_CREATE_FACTORY_DEBUG;
	}

	hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory));
	if (FAILED(hr))
		return false;

	//BOOL allowTearing = FALSE;
	//hr = m_factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
	//if (SUCCEEDED(hr))
	//{
	//	m_features.set(EXT_TEARING, allowTearing == TRUE);
	//}

	IDXGIAdapter1* pAdapter = NULL;
	hr = m_factory->EnumAdapters1(0, &pAdapter);
	if (FAILED(hr))
	{
		D3D12_RELEASE(pAdapter);
		//hr = m_factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter));
		//if (FAILED(hr))
		//{
		//	D3D12_RELEASE(pAdapter);
		//	return false;
		//}
	}

	if (pAdapter == NULL)
		return false;

	hr = pAdapter->QueryInterface(IID_PPV_ARGS(&m_adapter));
	D3D12_RELEASE(pAdapter);
	if (FAILED(hr))
		return false;

	IDXGIOutput* pOutput = NULL;
	hr = m_adapter->EnumOutputs(0, &pOutput);
	if (FAILED(hr))
		return false;

	hr = pOutput->QueryInterface(IID_PPV_ARGS(&m_output));
	D3D12_RELEASE(pOutput);
	if (FAILED(hr))
		return false;

	hr = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
	if (FAILED(hr))
		return false;

	// 
	// create queue

	// set info
	m_info.constantBufferMemoryAlignment	= D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	m_info.maxTargetWidth					= D3D12_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH;
	m_info.maxTargetHeight					= D3D12_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH;
	m_info.maxTargetArraySize				= D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
	m_info.maxColorSampleCount				= D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT;
	m_info.maxDepthSampleCount				= D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT;
	m_info.maxStencilSampleCount			= D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT;

	return true;
}

void D3D12_Device::term()
{
	D3D12_RELEASE(m_output);
	D3D12_RELEASE(m_adapter);
	D3D12_RELEASE(m_device);
	D3D12_RELEASE(m_factory);
}

D3D12_DescriptorHeap* D3D12_Device::getDescriptorHeap(size_t index)
{
	assert(index < m_heaps.size());
	return m_heaps[index];
}

//D3D12_Device*	gD3D12Device()
//{
//	return gGraphics.getDevice()->cast<D3D12_Device>();
//}
//
//ID3D12Device*	gD3D11NativeDevice()
//{
//	D3D12_Device* device = gGraphics.getDevice()->cast<D3D12_Device>();
//	if (device)
//		return device->getDevice();
//
//	return NULL;
//}
//
//D3D12_Device::D3D12_Device()
//	: m_device(NULL)
//	, m_debuger(NULL)
//{
//	D3D12GetDebugInterface(IID_PPV_ARGS(&m_debuger));
//	D3D12_CHECK(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)), "D3D12CreateDevice fail!");
//}
//
//D3D12_Device::~D3D12_Device()
//{
//	D3D12_RELEASE(m_device);
//}
//
//GpuBuffer* D3D12_Device::newBuffer(const BufferDesc& desc)
//{
//	return new D3D12_Buffer(m_device, desc);
//}
//
//Texture* D3D12_Device::newTexture(const TextureDesc& desc)
//{
//	return new D3D12_Texture(m_device, desc);
//}
//
//VertexLayout* D3D12_Device::newVertexLayout(const VertexElement* elements, size_t count)
//{
//	return NULL;
//}
//
//ShaderStage* D3D12_Device::newShader()
//{
//	return NULL;
//}
//
//ShaderProgram* D3D12_Device::newProgram()
//{
//	return NULL;
//}
//
//Pipeline* D3D12_Device::newPipeline(const PipelineDesc& desc)
//{
//	return NULL;
//}
//
//DescriptorSet* D3D12_Device::newDescriptorSet(ShaderStage* prog)
//{
//	return NULL;
//}
//
//CommandBuffer* D3D12_Device::newCommandBuffer()
//{
//	return new D3D12_CommandBuffer(m_device, m_allocator);
//}
//
//CommandQueue* D3D12_Device::newCommandQueue()
//{
//	return new D3D12_CommondQueue(m_device);
//}
//
//FrameBuffer* D3D12_Device::newFrameBuffer()
//{
//	return new D3D12_FrameBuffer();
//}
//
//SwapChain* D3D12_Device::newSwapChain(const SwapChainDesc& desc)
//{
//	return new D3D12_SwapChain(desc);
//}

CUTE_NS_END
