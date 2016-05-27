#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class D3D12_CommandBuffer;
class CU_D3D12_API D3D12_Pipeline : Pipeline
{
public:
	virtual ~D3D12_Pipeline(){}

	virtual void bind(D3D12_CommandBuffer* cmdBuffer) = 0;
};

class CU_D3D12_API D3D12_GraphicsPipeline : public Pipeline
{
public:
	D3D12_GraphicsPipeline(ID3D12Device* device, const PipelineDesc& desc);
	~D3D12_GraphicsPipeline();

	void bind(D3D12_CommandBuffer* cmdBuffer);

private:
	typedef std::map<uint32_t, ID3D12PipelineState*> PipelineMap;
	PipelineMap m_pipelines;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;

	ID3D12RootSignature* m_rootSignature;
};

class CU_D3D12_API D3D12_ComputePipeline : public Pipeline
{
public:
	D3D12_ComputePipeline(ID3D12Device* device, const PipelineDesc& desc);
	~D3D12_ComputePipeline();

	void bind(D3D12_CommandBuffer* cmdBuffer);

private:
	ID3D12PipelineState* m_pipeline;
};

CU_NS_END