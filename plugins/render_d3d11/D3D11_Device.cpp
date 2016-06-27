#pragma once
#include "Engine.h"
#include "D3D11_Device.h"
#include "D3D11_Buffer.h"
#include "D3D11_Texture.h"
#include "D3D11_Program.h"
#include "D3D11_Pipeline.h"
#include "D3D11_SwapChain.h"
#include "D3D11_FrameBuffer.h"
#include "D3D11_InputLayout.h"
#include "D3D11_RenderState.h"
#include "D3D11_CommandBuffer.h"
#include "D3D11_DescriptorSet.h"

CU_NS_BEGIN

D3D11_Device*	gD3D11Device()
{
	return gGraphics.getDevice()->cast<D3D11_Device>();
}

ID3D11DeviceN*	gD3D11NativeDevice()
{
	D3D11_Device* device = gGraphics.getDevice()->cast<D3D11_Device>();
	if (device)
		return device->getDevice();

	return NULL;
}

ID3D11ContextN* gD3D11NativeContext()
{
	D3D11_Device* device = gGraphics.getDevice()->cast<D3D11_Device>();
	if (device)
		return device->getContext();

	return NULL;
}

D3D11_Device::D3D11_Device()
	: m_device(NULL)
	, m_context(NULL)
	, m_factory(NULL)
	, m_adapter(NULL)
	, m_linkage(NULL)
	, m_layoutMax(0)
	, m_shaderID(0)
	, m_layoutID(0)
{
	HRESULT hr;

	// create factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactoryN), (void**)&m_factory);
	D3D11_CHECK(hr, "CreateDXGIFactory fail!");

	// create adapter, find first adapter
	UINT32 adapterIdx = 0;
	IDXGIAdapter* adapter = NULL;
	while ((hr = m_factory->EnumAdapters(adapterIdx, &adapter)) != DXGI_ERROR_NOT_FOUND)
	{
		if (FAILED(hr))
		{
			if (adapter)
				adapter->Release();
			++adapterIdx;
		}
		else
		{
			m_adapter = adapter;
			break;
		}
	}

	if (!m_adapter)
		throw std::runtime_error("enumerate adapters failed.");

	// create device
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1 
	};

	UINT flags = 0;
#ifdef CU_DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	hr = D3D11CreateDevice(m_adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, featureLevels, CU_ARRAY_SIZE(featureLevels), D3D11_SDK_VERSION, &m_device, NULL, &m_context);
	D3D11_CHECK( hr, "D3D11CreateDevice fail!");

	// create linkage
	if (m_device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
	{
		hr = m_device->CreateClassLinkage(&m_linkage);
		D3D11_CHECK(hr, "CreateClassLinkage fail!");
	}
}

D3D11_Device::~D3D11_Device()
{
	D3D11_RELEASE(m_device);
}

GpuBuffer* D3D11_Device::newBuffer(const BufferDesc& desc)
{
	return new D3D11_Buffer(desc, m_device);
}

Texture* D3D11_Device::newTexture(const TextureDesc& desc)
{
	return new D3D11_Texture(desc, m_device);
}

FrameBuffer* D3D11_Device::newFrameBuffer()
{
	return new D3D11_FrameBuffer();
}

SwapChain* D3D11_Device::newSwapChain(const SwapChainDesc& desc)
{
	return new D3D11_SwapChain(desc, m_factory, m_device);
}

InputLayout* D3D11_Device::newInputLayout(const InputElement* elements, size_t count)
{
	++m_layoutID;
	if (m_layoutID == 0)
		m_layoutID = 1;
	return new D3D11_InputLayout(m_layoutID, elements, count);
}

ShaderStage* D3D11_Device::newShader()
{
	++m_shaderID;
	if (m_shaderID == 0)
		m_shaderID = 1;
	return new D3D11_Shader(m_shaderID);
}

ShaderProgram* D3D11_Device::newProgram()
{
	return new D3D11_Program();
}

Pipeline* D3D11_Device::newPipeline(const PipelineDesc& desc)
{
	if (desc.program == NULL)
		return NULL;
	return new D3D11_Pipeline(this, desc);
}

DescriptorSet* D3D11_Device::newDescriptorSet(Pipeline* pipeline)
{
	return new D3D11_DescriptorSet(((D3D11_Pipeline*)pipeline)->getProgram(), this);
}

CommandBuffer* D3D11_Device::newCommandBuffer()
{
	// todo:deferred??
	return new D3D11_CommandBuffer(m_device, m_context);
}

CommandQueue* D3D11_Device::newCommandQueue()
{
	return NULL;
}

D3D11_BlendState* D3D11_Device::getBlendState(const BlendDesc& desc)
{
	return m_blends.obtain(m_device, desc);
}

D3D11_SamplerState* D3D11_Device::getSamplerState(const SamplerDesc& desc)
{
	return m_samplers.obtain(m_device, desc);
}

D3D11_RasterizerState* D3D11_Device::getRasterizerState(const RasterizerDesc& desc)
{
	return m_rasterizers.obtain(m_device, desc);
}

D3D11_DepthStencilState* D3D11_Device::getDepthStencilState(const DepthStencilDesc& desc)
{
	return m_depthStencils.obtain(m_device, desc);
}

ID3D11InputLayout* D3D11_Device::getInputLayout(D3D11_Shader* vs, D3D11_InputLayout* layout)
{
	if (vs == NULL || layout == NULL)
		return NULL;

	uint64_t uid = vs->getID();
	uid = (uid << 32) + layout->getID();
	LayoutMap::iterator itor = m_layouts.find(uid);
	if (itor != m_layouts.end())
	{
		return itor->second;
	}

	// 先清理不再使用的
	if (m_layoutMax > 0 && m_layouts.size() > m_layoutMax)
	{
		ID3D11InputLayout* tmp_layout;
		ULONG tmp_count;
		for (LayoutMap::iterator itor = m_layouts.begin(); itor != m_layouts.end();)
		{
			tmp_layout = itor->second;
			tmp_count = tmp_layout->AddRef();
			tmp_layout->Release();
			if (tmp_count == 2)
			{// 已经没有用了
				tmp_layout->Release();
				itor = m_layouts.erase(itor);
			}
			else
			{
				++itor;
			}
		}
	}

	// 创建
	ID3D11InputLayout* dx_layout = layout->createLayout(m_device, vs->getCode());
	if (!dx_layout)
		return NULL;

	m_layouts[uid] = dx_layout;

	return dx_layout;
}

CU_NS_END