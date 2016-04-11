#include "D3D11_RenderState.h"
#include "D3D11_Mapping.h"

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