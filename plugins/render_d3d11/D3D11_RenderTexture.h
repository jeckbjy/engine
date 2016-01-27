#pragma once
#include "D3D11_API.h"
#include <vector>

CU_NS_BEGIN

class CU_D3D11_API D3D11RenderTexture : public D3D11RenderTarget
{
public:
	D3D11RenderTexture();
	~D3D11RenderTexture();

	void bind(ID3D11DeviceContextN* context);

protected:
	typedef std::vector<ID3D11RenderTargetView*>	RenderTargetViewVec;
	typedef std::vector<ID3D11UnorderedAccessView*>	UnorderedAccessViewVec;
	ID3D11DepthStencilView*		_dsv;
	RenderTargetViewVec			_rtv;
	UnorderedAccessViewVec		_uav;
};

CU_NS_END