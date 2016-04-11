#include "D3D11_Graphics.h"

CU_NS_BEGIN

D3D11Graphics::D3D11Graphics()
{

}

D3D11Graphics::~D3D11Graphics()
{

}

ID3D11RasterizerState* D3D11Graphics::getRasterizerState(const RasterizerDesc& desc)
{
	ID3D11RasterizerState* state = NULL;
	// 先查找，查找不到创建

	return NULL;
}

ID3D11BlendState* D3D11Graphics::getBlendState(const BlendDesc& desc)
{
	return NULL;
}

ID3D11DepthStencilState* D3D11Graphics::getDepthStencilState(const DepthStencilDesc& desc)
{
	return NULL;
}

ID3D11SamplerState* D3D11Graphics::getSamplerState(const SamplerDesc& desc)
{
	return NULL;
}

CU_NS_END