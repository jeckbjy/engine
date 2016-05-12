#include "D3D11_RenderState.h"
#include "D3D11_Mapping.h"

CU_NS_BEGIN

D3D11RasterizerState::D3D11RasterizerState(ID3D11DeviceN* device, const RasterizerDesc& desc)
{
	D3D11_RASTERIZER_DESC info;
	D3D11Mapping::fillRasterizer(info, desc);
	device->CreateRasterizerState(&info, &m_state);
}

D3D11RasterizerState::~D3D11RasterizerState()
{
	D3D11_RELEASE(m_state);
}

D3D11BlendState::D3D11BlendState(const BlendDesc& desc)
{

}

D3D11BlendState::~D3D11BlendState()
{

}

D3D11DepthStencilState::D3D11DepthStencilState(const DepthStencilDesc& desc)
{

}

D3D11DepthStencilState::~D3D11DepthStencilState()
{

}

D3D11SamplerState::D3D11SamplerState(const SamplerDesc& desc)
{

}

D3D11SamplerState::~D3D11SamplerState()
{

}

CU_NS_END
