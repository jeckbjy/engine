#include "D3D11_RenderTexture.h"

CU_NS_BEGIN

D3D11_RenderTexture::D3D11_RenderTexture()
{

}

D3D11_RenderTexture::~D3D11_RenderTexture()
{

}

void D3D11_RenderTexture::bind(ID3D11ContextN* context)
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