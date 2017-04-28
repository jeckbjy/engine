#include "D3D12_Pipeline.h"
#include "D3D12_Mapping.h"
#include "D3D12_Device.h"
#include "D3D12_Program.h"
#include "D3D12_VertexLayout.h"
#include "D3D12_CommandBuffer.h"

CUTE_NS_BEGIN

D3D12_GraphicsPipeline::D3D12_GraphicsPipeline(ID3D12Device* device, const PipelineDesc& desc)
	: m_rootSignature(NULL)
{
	const RenderStateDesc* states = desc.states;
	// 根据shader创建
	ID3D10Blob *sig, *err;
	D3D12_ROOT_SIGNATURE_DESC sigDesc;
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &err);
	device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	
	D3D12_RELEASE(sig);
	//D3D12_RELEASE(err);

	// 填充固定的
	m_desc.PrimitiveTopologyType = D3D12_Mapping::getTopologyType(states->topology);
	m_desc.pRootSignature = m_rootSignature;
	
	//D3D12Mapping::fillShader(m_desc.VS, desc.vs);
	//D3D12Mapping::fillShader(m_desc.HS, desc.hs);
	//D3D12Mapping::fillShader(m_desc.DS, desc.ds);
	//D3D12Mapping::fillShader(m_desc.GS, desc.gs);
	//D3D12Mapping::fillShader(m_desc.PS, desc.ps);

	D3D12_Mapping::fillRasterizerState(m_desc.RasterizerState, states->rasterizer);
	D3D12_Mapping::fillDepthStencilState(m_desc.DepthStencilState, states->depthStencil);
	D3D12_Mapping::fillBlendState(m_desc.BlendState, states->blend);
}

D3D12_GraphicsPipeline::~D3D12_GraphicsPipeline()
{
	D3D12_RELEASE(m_rootSignature);
}

void D3D12_GraphicsPipeline::bind(D3D12_CommandBuffer* cmdBuffer)
{
	D3D12_Device* device = gD3D12Device();
	// 查找
	D3D12_VertexLayout* layout = cmdBuffer->getLayout();
	if (!layout)
		return;

	ID3D12PipelineState* pso = NULL;
	PipelineMap::iterator itor = m_pipelines.find(layout->getID());
	if (itor == m_pipelines.end())
	{
		// 创建
		layout->fillLayout(m_desc.InputLayout);
		HRESULT hr = device->getDevice()->CreateGraphicsPipelineState(&m_desc, IID_PPV_ARGS(&pso));
		if (FAILED(hr))
			return;

		m_pipelines[layout->getID()] = pso;
	}
	else
	{
		pso = itor->second;
	}

	cmdBuffer->native()->SetPipelineState(pso);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
D3D12_ComputePipeline::D3D12_ComputePipeline(ID3D12Device* device, const PipelineDesc& desc)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pso = {};
	pso.pRootSignature = NULL;

	//D3D12Mapping::fillShader(pso.CS, desc.cs);

	device->CreateComputePipelineState(&pso, IID_PPV_ARGS(&m_pipeline));
}

D3D12_ComputePipeline::~D3D12_ComputePipeline()
{
	D3D12_RELEASE(m_pipeline);
}

void D3D12_ComputePipeline::bind(D3D12_CommandBuffer* cmdBuffer)
{
	if (!m_pipeline)
		return;

	cmdBuffer->native()->SetPipelineState(m_pipeline);
}

CUTE_NS_END
