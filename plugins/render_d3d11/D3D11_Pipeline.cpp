#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"
#include "D3D11_InputLayout.h"
#include "D3D11_CommandBuffer.h"
#include "D3D11_RenderState.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

D3D11_Pipeline::D3D11_Pipeline(D3D11_Device* device, const PipelineDesc& desc)
	: m_blend(NULL)
	, m_rasterizer(NULL)
	, m_depthStencil(NULL)
	, m_curlayout(NULL)
{
	const RenderStateDesc* stateDesc = desc.states;
	if (stateDesc)
	{
		m_blend = device->getBlendState(stateDesc->blend);
		m_rasterizer = device->getRasterizerState(stateDesc->rasterizer);
		m_depthStencil = device->getDepthStencilState(stateDesc->depthStencil);
	}
	m_program = (D3D11_Program*)desc.program;
}

D3D11_Pipeline::~D3D11_Pipeline()
{
	CU_SAFE_RELEASE(m_blend);
	CU_SAFE_RELEASE(m_rasterizer);
	CU_SAFE_RELEASE(m_depthStencil);
}

void D3D11_Pipeline::bind(D3D11_CommandBuffer* cmdBuffer)
{
	if (!m_program)
		return;

	D3D11_Device* device = gD3D11Device();
	if (!device)
		return;

	ID3D11ContextN* context = cmdBuffer->getContext();
	D3D11_Shader* vs = m_program->getVS();
	if (vs)
	{
		// graphics pipeline
		D3D11_InputLayout* layout = cmdBuffer->getLayout();
		if (m_curlayout != layout)
			m_d3dLayout = device->getInputLayout(vs, layout);
		if (m_d3dLayout)
			context->IASetInputLayout(m_d3dLayout.get());

		context->RSSetState(m_rasterizer->getState());
		context->OMSetDepthStencilState(m_depthStencil->getState(), cmdBuffer->getStencilRef());
		context->OMSetBlendState(m_blend->getState(), cmdBuffer->getFactors(), cmdBuffer->getSampleMask());
	}

	m_program->bind(context);
}

CU_NS_END