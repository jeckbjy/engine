#include "D3D12_Pipeline.h"
#include "D3D12_Mapping.h"
#include "D3D12_Device.h"
#include "D3D12_Program.h"
#include "D3D12_InputLayout.h"
#include "D3D12_CommandBuffer.h"

CU_NS_BEGIN

D3D12GraphicsPipeline::D3D12GraphicsPipeline(ID3D12Device* device, const GraphicsPipelineDesc& desc)
	: m_rootSignature(NULL)
{
	// 根据shader创建
	ID3D10Blob *sig, *err;
	D3D12_ROOT_SIGNATURE_DESC sigDesc;
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &err);
	device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	
	D3D12_RELEASE(sig);
	//D3D12_RELEASE(err);

	// 填充固定的
	m_desc.PrimitiveTopologyType = D3D12Mapping::getTopologyType(desc.topology);
	m_desc.pRootSignature = m_rootSignature;
	
	D3D12Mapping::fillShader(m_desc.VS, desc.vs);
	D3D12Mapping::fillShader(m_desc.HS, desc.hs);
	D3D12Mapping::fillShader(m_desc.DS, desc.ds);
	D3D12Mapping::fillShader(m_desc.GS, desc.gs);
	D3D12Mapping::fillShader(m_desc.PS, desc.ps);

	D3D12Mapping::fillRasterizerState(m_desc.RasterizerState, desc.rasterizer);
	D3D12Mapping::fillDepthStencilState(m_desc.DepthStencilState, desc.depthStencil);
	D3D12Mapping::fillBlendState(m_desc.BlendState, desc.blend);
}

D3D12GraphicsPipeline::~D3D12GraphicsPipeline()
{
	D3D12_RELEASE(m_rootSignature);
}

void D3D12GraphicsPipeline::bind(D3D12CommandBuffer* cmdBuffer)
{
	D3D12Device* device = gD3D12Device();
	// 查找
	D3D12InputLayout* layout = cmdBuffer->getLayout();
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
D3D12ComputePipeline::D3D12ComputePipeline(ID3D12Device* device, const ComputePipelineDesc& desc)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pso = {};
	pso.pRootSignature = NULL;

	D3D12Mapping::fillShader(pso.CS, desc.cs);

	device->CreateComputePipelineState(&pso, IID_PPV_ARGS(&m_pipeline));
}

D3D12ComputePipeline::~D3D12ComputePipeline()
{
	D3D12_RELEASE(m_pipeline);
}

void D3D12ComputePipeline::bind(D3D12CommandBuffer* cmdBuffer)
{
	if (!m_pipeline)
		return;

	cmdBuffer->native()->SetPipelineState(m_pipeline);
}

CU_NS_END