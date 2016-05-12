#pragma once
#include "D3D11_API.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Graphics : public Graphics
{
public:
	D3D11Graphics();
	~D3D11Graphics();

	Device* getDevice() { return m_device; }

	ID3D11RasterizerState*		getRasterizerState(const RasterizerDesc& desc);
	ID3D11BlendState*			getBlendState(const BlendDesc& desc);
	ID3D11DepthStencilState*	getDepthStencilState(const DepthStencilDesc& desc);
	ID3D11SamplerState*			getSamplerState(const SamplerDesc& desc);

private:
	D3D11Device* m_device;
};

CU_NS_END
