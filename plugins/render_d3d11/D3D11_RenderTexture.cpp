#include "D3D11_RenderTexture.h"

CU_NS_BEGIN

D3D11RenderTexture::D3D11RenderTexture()
{

}

D3D11RenderTexture::~D3D11RenderTexture()
{

}

void D3D11RenderTexture::bind(ID3D11DeviceContextN* context)
{
	if (_uav.empty())
	{
		context->OMSetRenderTargets(_rtv.size(), &_rtv[0], _dsv);
	}
	else
	{
		//context->OMSetRenderTargetsAndUnorderedAccessViews(_rtv.size(), &_rtv[0], _dsv, 0, _uav.size(), &_uav[0], );
	}
}

CU_NS_END