#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"
#include "D3D11_InputLayout.h"
#include "D3D11_CommandBuffer.h"
#include "D3D11_RenderState.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

D3D11GraphicsPipeline::D3D11GraphicsPipeline(D3D11Device* device, const GraphicsPipelineDesc& desc)
{
	m_blend = device->getBlendState(desc.blend);
	m_rasterizer = device->getRasterizerState(desc.rasterizer);
	m_depthStencil = device->getDepthStencilState(desc.depthStencil);

	m_vs = (D3D11Program*)desc.vs;
	m_hs = (D3D11Program*)desc.hs;
	m_ds = (D3D11Program*)desc.ds;
	m_gs = (D3D11Program*)desc.gs;
	m_ps = (D3D11Program*)desc.ps;

	m_curLayout = NULL;
}

D3D11GraphicsPipeline::~D3D11GraphicsPipeline()
{
	m_blend->release();
	m_rasterizer->release();
	m_depthStencil->release();
}

void D3D11GraphicsPipeline::bind(D3D11CommandBuffer* cmdBuffer)
{
	D3D11Device* device = gD3D11Device();
	if (!device)
		return;

	ID3D11ContextN* context = cmdBuffer->getContext();

	D3D11InputLayout* layout = cmdBuffer->getLayout();
	if (m_curLayout != layout)
		m_d3dLayout = device->getInputLayout(m_vs, layout);
	if (m_d3dLayout)
		context->IASetInputLayout(m_d3dLayout.get());

	context->RSSetState(m_rasterizer->getState());
	context->OMSetDepthStencilState(m_depthStencil->getState(), cmdBuffer->getStencilRef());
	context->OMSetBlendState(m_blend->getState(), cmdBuffer->getFactors(), cmdBuffer->getSampleMask());

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
	m_cs = (D3D11Program*)desc.cs;
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