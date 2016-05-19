#pragma once
#include "D3D11_Device.h"
#include "D3D11_Buffer.h"
#include "D3D11_Texture.h"
#include "D3D11_Program.h"
#include "D3D11_Pipeline.h"
#include "D3D11_InputLayout.h"
#include "D3D11_RenderState.h"
#include "D3D11_CommandBuffer.h"

#include "Engine.h"

CU_NS_BEGIN

D3D11Device*	gD3D11Device()
{
	return gEngine.getDevice()->cast<D3D11Device>();
}

ID3D11DeviceN*	gD3D11NativeDevice()
{
	D3D11Device* device = gEngine.getDevice()->cast<D3D11Device>();
	if (device)
		return device->getDevice();

	return NULL;
}

ID3D11ContextN* gD3D11NativeContext()
{
	D3D11Device* device = gEngine.getDevice()->cast<D3D11Device>();
	if (device)
		return device->getContext();

	return NULL;
}

D3D11Device::D3D11Device()
	: m_device(NULL)
	, m_factory(NULL)
	, m_context(NULL)
	, m_linkage(NULL)
	, m_layoutMax(0)
	, m_programID(0)
	, m_layoutID(0)
{
	D3D_FEATURE_LEVEL featureLevels[6] = 
	{
		//D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1 
	};

	HRESULT hr;
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 0, featureLevels, 6, D3D11_SDK_VERSION, &m_device, NULL, &m_context);
	D3D11_CHECK( hr, "D3D11CreateDevice fail!");

	hr = CreateDXGIFactory(__uuidof(IDXGIFactoryN), (void**)&m_factory);
	D3D11_CHECK( hr, "CreateDXGIFactory fail!");

	if (m_device->GetFeatureLevel() == D3D_FEATURE_LEVEL_11_0)
	{
		hr = m_device->CreateClassLinkage(&m_linkage);
		D3D11_CHECK(hr, "CreateClassLinkage fail!");
	}
}

D3D11Device::~D3D11Device()
{
	D3D11_RELEASE(m_device);
}

GpuBuffer* D3D11Device::newBuffer(const BufferDesc& desc)
{
	return new D3D11Buffer(desc, m_device);
}

Texture* D3D11Device::newTexture(const TextureDesc& desc)
{
	return new D3D11Texture(desc, m_device);
}

RenderTarget* D3D11Device::newRenderWindow(Window* hwnd)
{
	return NULL;
}

RenderTarget* D3D11Device::newRenderTexture(Texture* rtv, Texture* dsv)
{
	return NULL;
}

InputLayout* D3D11Device::newInputLayout(const InputElement* elements, size_t count)
{
	++m_layoutID;
	if (m_layoutID == 0)
		m_layoutID = 1;
	return new D3D11InputLayout(m_layoutID, elements, count);
}

Program* D3D11Device::newProgram()
{
	++m_programID;
	if (m_programID == 0)
		m_programID = 1;
	return new D3D11Program(m_programID);
}

Pipeline* D3D11Device::newPipeline(const GraphicsPipelineDesc& desc)
{
	return new D3D11GraphicsPipeline(this, desc);
}

Pipeline* D3D11Device::newPipeline(const ComputePipelineDesc& desc)
{
	return new D3D11ComputePipeline(desc);
}

DescriptorSet* D3D11Device::newDescriptorSet(Program* prog)
{
	return NULL;
}

CommandBuffer* D3D11Device::newCommandBuffer()
{
	return new D3D11CommandBuffer();
}

CommandQueue* D3D11Device::newCommandQueue()
{
	return NULL;
}

D3D11BlendState* D3D11Device::getBlendState(const BlendDesc& desc)
{
	return m_blends.obtain(m_device, desc);
}

D3D11SamplerState* D3D11Device::getSamplerState(const SamplerDesc& desc)
{
	return m_samplers.obtain(m_device, desc);
}

D3D11RasterizerState* D3D11Device::getRasterizerState(const RasterizerDesc& desc)
{
	return m_rasterizers.obtain(m_device, desc);
}

D3D11DepthStencilState* D3D11Device::getDepthStencilState(const DepthStencilDesc& desc)
{
	return m_depthStencils.obtain(m_device, desc);
}

ID3D11InputLayout* D3D11Device::getInputLayout(D3D11Program* vs, D3D11InputLayout* layout)
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