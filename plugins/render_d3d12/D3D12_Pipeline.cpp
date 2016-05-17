#include "D3D12_Pipeline.h"
#include "D3D12_Mapping.h"
#include "D3D12_Device.h"
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
	D3D12Mapping::fillRasterizerState(m_desc.RasterizerState, desc.rasterizer);
	D3D12Mapping::fillDepthStencilState(m_desc.DepthStencilState, desc.depthStencil);
	D3D12Mapping::fillBlendState(m_desc.BlendState, desc.blend);
}

D3D12GraphicsPipeline::~D3D12GraphicsPipeline()
{
	D3D12_RELEASE(m_rootSignature);
}

void D3D12GraphicsPipeline::bind(D3D12CommandBuffer* cmd)
{
	// 查找
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

D3D12ComputePipeline::D3D12ComputePipeline(ID3D12Device* device, const ComputePipelineDesc& desc)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pso = {};
	pso.pRootSignature = NULL;
	//pso.CS.
	device->CreateComputePipelineState(&pso, IID_PPV_ARGS(&m_pipeline));
}

D3D12ComputePipeline::~D3D12ComputePipeline()
{
	D3D12_RELEASE(m_pipeline);
}

//D3D12Pipeline::D3D12Pipeline(ID3D12Device* device, const GraphicsPipelineDesc& desc)
//	: m_pipeline(NULL)
//{
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso;
//	ZeroMemory(&pso, sizeof(pso));
//
//	D3D12_ROOT_SIGNATURE_DESC sign_desc;
//	//convert
//	pso.PrimitiveTopologyType = D3D12Mapping::getTopologyType(desc.topology);
//	D3D12Mapping::fillRasterizerState(pso.RasterizerState, desc.rasterizer);
//	D3D12Mapping::fillDepthStencilState(pso.DepthStencilState, desc.depthStencil);
//
//	D3D12_CHECK(device->CreateGraphicsPipelineState(&pso, IID_PPV_ARGS(&m_pipeline)), "CreateGraphicsPipelineState fail!");
//}
//
//D3D12Pipeline::D3D12Pipeline(ID3D12Device* device, const ComputePipelineDesc& desc)
//	: m_pipeline(NULL)
//{
//	D3D12_COMPUTE_PIPELINE_STATE_DESC pso;
//	ZeroMemory(&pso, sizeof(pso));
//
//	D3D12_CHECK(device->CreateComputePipelineState(&pso, IID_PPV_ARGS(&m_pipeline)), "CreateComputePipelineState fail!");
//}
//
//D3D12Pipeline::~D3D12Pipeline()
//{
//	D3D12_RELEASE(m_pipeline);
//}

CU_NS_END