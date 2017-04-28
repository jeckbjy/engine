#include "D3D11_RenderState.h"
#include "D3D11_Mapping.h"

CUTE_NS_BEGIN

D3D11_RasterizerState::D3D11_RasterizerState(ID3D11DeviceN* device, const RasterizerDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_RASTERIZER_DESC info;
	D3D11_Mapping::fillRasterizer(info, desc);
	device->CreateRasterizerState(&info, &m_state);
}

D3D11_RasterizerState::~D3D11_RasterizerState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11_RasterizerState::equal(const RasterizerDesc& desc) const
{
	return m_desc == desc;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11_BlendState::D3D11_BlendState(ID3D11DeviceN* device, const BlendDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_BLEND_DESC info;
	D3D11_Mapping::fillBlend(info, desc);
	device->CreateBlendState(&info, &m_state);
}

D3D11_BlendState::~D3D11_BlendState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11_BlendState::equal(const BlendDesc& desc) const
{
	return m_desc == desc;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11_DepthStencilState::D3D11_DepthStencilState(ID3D11DeviceN* device, const DepthStencilDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_DEPTH_STENCIL_DESC info;
	D3D11_Mapping::fillDepthStencil(info, desc);
}

D3D11_DepthStencilState::~D3D11_DepthStencilState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11_DepthStencilState::equal(const DepthStencilDesc& desc) const
{
	return m_desc == desc;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11_SamplerState::D3D11_SamplerState(ID3D11DeviceN* device, const SamplerDesc& desc, uint32_t hash)
	: m_state(NULL)
{
	m_hash = hash;
	m_desc = desc;
	D3D11_SAMPLER_DESC info;
	D3D11_Mapping::fillSampler(info, desc);
	device->CreateSamplerState(&info, &m_state);
}

D3D11_SamplerState::~D3D11_SamplerState()
{
	D3D11_RELEASE(m_state);
}

bool D3D11_SamplerState::equal(const SamplerDesc& desc) const
{
	return m_desc == desc;
}

CUTE_NS_END
