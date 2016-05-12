#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11GraphicsPipeline::D3D11GraphicsPipeline(const GraphicsPipelineDesc& desc)
{
}

D3D11GraphicsPipeline::~D3D11GraphicsPipeline()
{
}

void D3D11GraphicsPipeline::bind(ID3D11ContextN* context, const float factors[4], size_t stencilref)
{
	//if (m_prog == NULL)
	//	return;
	//context->OMSetDepthStencilState(_depth_stencil, stencilref);
	//context->OMSetBlendState(_blend, factors, _mask);
	//context->RSSetState(_rasterizer);
	//gD3D11Context->VSSetSamplers();	// 采样,每个阶段都有
}

CU_NS_END