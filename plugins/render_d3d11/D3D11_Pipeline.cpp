#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11Pipeline::D3D11Pipeline(const PIPELINE_DESC& desc)
:Pipeline(desc)
, _depth_stencil(NULL)
, _blend(NULL)
, _rasterizer(NULL)
, _sampler(NULL)
, m_prog(NULL)
{

}

D3D11Pipeline::~D3D11Pipeline()
{
	D3D11_RELEASE(_depth_stencil);
	D3D11_RELEASE(_blend);
	D3D11_RELEASE(_rasterizer);
}

void D3D11Pipeline::bind(ID3D11DeviceContextN* context, const float factors[4], size_t stencilref)
{
	if (m_prog == NULL)
		return;
	m_prog->bind(context);
	context->OMSetDepthStencilState(_depth_stencil, stencilref);
	context->OMSetBlendState(_blend, factors, _mask);
	context->RSSetState(_rasterizer);
	//gD3D11Context->VSSetSamplers();	// 采样,每个阶段都有
}

CU_NS_END