#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"
#include "D3D11_InputLayout.h"
#include "D3D11_CommandBuffer.h"

CU_NS_BEGIN

D3D11GraphicsPipeline::D3D11GraphicsPipeline(const GraphicsPipelineDesc& desc)
{
}

D3D11GraphicsPipeline::~D3D11GraphicsPipeline()
{
}

void D3D11GraphicsPipeline::bind(D3D11CommandBuffer* cmdBuffer)
{
	ID3D11ContextN* context = cmdBuffer->getContext();

	D3D11InputLayout* layout = cmdBuffer->getLayout();
	if (layout)
	{
		ID3D11InputLayout* dx_layout = layout->getLayout(m_vs);
		if (dx_layout)
			context->IASetInputLayout(dx_layout);
	}

	context->OMSetDepthStencilState(m_depthStencil, cmdBuffer->getStencilRef());
	context->OMSetBlendState(m_blend, cmdBuffer->getFactors(), cmdBuffer->getSampleMask());
	context->RSSetState(m_rasterizer);

	// context->VSSetSamplers();
	// set shader
	if (m_vs)
		context->VSSetShader(m_vs->getShader<ID3D11VertexShader>(), NULL, 0);
	
	if (m_hs)
		context->HSSetShader(m_hs->getShader<ID3D11HullShader>(), NULL, 0);

	if (m_ds)
		context->DSSetShader(m_ds->getShader<ID3D11DomainShader>(), NULL, 0);

	if (m_gs)
		context->GSSetShader(m_gs->getShader<ID3D11GeometryShader>(), NULL, 0);

	if (m_ps)
		context->PSSetShader(m_ps->getShader<ID3D11PixelShader>(), NULL, 0);

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D11ComputePipeline::D3D11ComputePipeline(const ComputePipelineDesc& desc)
{

}

D3D11ComputePipeline::~D3D11ComputePipeline()
{

}

void D3D11ComputePipeline::bind(D3D11CommandBuffer* cmdBuffer)
{
	ID3D11ContextN* context = cmdBuffer->getContext();
	if (m_cs)
		context->CSSetShader(m_cs->getShader<ID3D11ComputeShader>(), NULL, 0);
}

CU_NS_END