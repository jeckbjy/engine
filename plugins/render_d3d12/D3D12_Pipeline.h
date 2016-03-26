#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

// InputLayout和RenderPass如何处理呢？？
class CU_D3D12_API D3D12GraphicsPipeline : public Pipeline
{
public:
	D3D12GraphicsPipeline(ID3D12Device* device, const GraphicsPipelineDesc& desc);
	~D3D12GraphicsPipeline();

private:
	typedef std::map<uint32_t, ID3D12PipelineState*> PipelineMap;
	PipelineMap m_pipelines;
	GraphicsPipelineDesc m_desc;
};

class CU_D3D12_API D3D12ComputePipeline : public Pipeline
{
public:
	D3D12ComputePipeline(ID3D12Device* device, const ComputePipelineDesc& desc);
	~D3D12ComputePipeline();

private:
	ID3D12PipelineState* m_pipeline;
};

// 
/*
问题：D3D12_GRAPHICS_PIPELINE_STATE_DESC的初始化
1:需要InputLayout，而这些数据的获得会延迟得到，与shader无关
2:D3D12_ROOT_SIGNATURE_DESC初始化：跟shader相关，应该可以通过Shader自动解析得到,类似PipelineLayout
*/
//class CU_D3D12_API D3D12GraphicsPipeline : public Pipeline
//{
//public:
//
//	//void bind(InputLayout* layout);
//
//private:
//	// 通过VertexLayout查找
//	typedef std::map<uint32_t, ID3D12PipelineState*> PSOMap;
//	// 需要持有多个？？通过Vertex查找
//	//ID3D12PipelineState* m_pso;
//};
//
//class CU_D3D12_API D3D12Pipeline : public Pipeline
//{
//public:
//	D3D12Pipeline(ID3D12Device* device, const GraphicsPipelineDesc& desc);
//	D3D12Pipeline(ID3D12Device* device, const ComputePipelineDesc& desc);
//	~D3D12Pipeline();
//
//private:
//	ID3D12PipelineState* m_pipeline;
//};

CU_NS_END
