#include "D3D12_Pipeline.h"
#include "D3D12_Mapping.h"

CU_NS_BEGIN

D3D12Pipeline::D3D12Pipeline(ID3D12Device* device, const GraphicsPipelineDesc& desc)
	: m_pipeline(NULL)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso;
	ZeroMemory(&pso, sizeof(pso));
	//convert
	pso.PrimitiveTopologyType = D3D12Mapping::getTopologyType(desc.topology);
	D3D12Mapping::fillRasterizerState(pso.RasterizerState, desc.rasterizer);
	D3D12Mapping::fillDepthStencilState(pso.DepthStencilState, desc.depthStencil);

	D3D12_CHECK(device->CreateGraphicsPipelineState(&pso, IID_PPV_ARGS(&m_pipeline)), "CreateGraphicsPipelineState fail!");
}

D3D12Pipeline::D3D12Pipeline(ID3D12Device* device, const ComputePipelineDesc& desc)
	: m_pipeline(NULL)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pso;
	ZeroMemory(&pso, sizeof(pso));

	D3D12_CHECK(device->CreateComputePipelineState(&pso, IID_PPV_ARGS(&m_pipeline)), "CreateComputePipelineState fail!");
}

D3D12Pipeline::~D3D12Pipeline()
{
	D3D12_RELEASE(m_pipeline);
}

CU_NS_END
