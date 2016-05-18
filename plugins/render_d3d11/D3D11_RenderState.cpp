#include "D3D11_RenderState.h"
#include "D3D11_Mapping.h"

CU_NS_BEGIN

D3D11RasterizerState::D3D11RasterizerState(ID3D11DeviceN* device, const RasterizerDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_RASTERIZER_DESC info;
	D3D11Mapping::fillRasterizer(info, desc);
	device->CreateRasterizerState(&info, &m_state);
}

D3D11RasterizerState::~D3D11RasterizerState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11RasterizerState::equal(const RasterizerDesc& desc) const
{
	return m_desc == desc;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11BlendState::D3D11BlendState(ID3D11DeviceN* device, const BlendDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_BLEND_DESC info;
	D3D11Mapping::fillBlend(info, desc);
	device->CreateBlendState(&info, &m_state);
}

D3D11BlendState::~D3D11BlendState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11BlendState::equal(const BlendDesc& desc) const
{
	return m_desc == desc;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11DepthStencilState::D3D11DepthStencilState(ID3D11DeviceN* device, const DepthStencilDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_DEPTH_STENCIL_DESC info;
	D3D11Mapping::fillDepthStencil(info, desc);
}

D3D11DepthStencilState::~D3D11DepthStencilState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11DepthStencilState::equal(const DepthStencilDesc& desc) const
{
	return m_desc == desc;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11SamplerState::D3D11SamplerState(ID3D11DeviceN* device, const SamplerDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_SAMPLER_DESC info;
	D3D11Mapping::fillSampler(info, desc);
	device->CreateSamplerState(&info, &m_state);
}

D3D11SamplerState::~D3D11SamplerState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11SamplerState::equal(const SamplerDesc& desc) const
{
	return m_desc == desc;
}

CU_NS_END