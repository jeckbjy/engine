#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

// È«¾Ö£¿£¿
class CU_D3D11_API D3D11Graphics : public Graphics
{
public:
	D3D11Graphics();
	~D3D11Graphics();

	ID3D11RasterizerState*		getRasterizerState(const RasterizerDesc& desc);
	ID3D11BlendState*			getBlendState(const BlendDesc& desc);
	ID3D11DepthStencilState*	getDepthStencilState(const DepthStencilDesc& desc);
	ID3D11SamplerState*			getSamplerState(const SamplerDesc& desc);

private:
	ID3D11DeviceN* m_device;
};

CU_NS_END
