#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class D3D12CommandBuffer;
class CU_D3D12_API D3D12Pipeline : Pipeline
{
public:
	virtual ~D3D12Pipeline(){}

	virtual void bind(D3D12CommandBuffer* cmdBuffer) = 0;
};

// InputLayout和RenderPass如何处理呢？？
class CU_D3D12_API D3D12GraphicsPipeline : public Pipeline
{
public:
	D3D12GraphicsPipeline(ID3D12Device* device, const GraphicsPipelineDesc& desc);
	~D3D12GraphicsPipeline();

	void bind(D3D12CommandBuffer* cmdBuffer);

private:
	typedef std::map<uint32_t, ID3D12PipelineState*> PipelineMap;
	PipelineMap m_pipelines;

	//GraphicsPipelineDesc m_desc;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;

	ID3D12RootSignature* m_rootSignature;
};

class CU_D3D12_API D3D12ComputePipeline : public Pipeline
{
public:
	D3D12ComputePipeline(ID3D12Device* device, const ComputePipelineDesc& desc);
	~D3D12ComputePipeline();

	void bind(D3D12CommandBuffer* cmdBuffer);

private:
	ID3D12PipelineState* m_pipeline;
};

CU_NS_END
