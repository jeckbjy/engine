#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

// InputLayout��RenderPass��δ����أ���
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
���⣺D3D12_GRAPHICS_PIPELINE_STATE_DESC�ĳ�ʼ��
1:��ҪInputLayout������Щ���ݵĻ�û��ӳٵõ�����shader�޹�
2:D3D12_ROOT_SIGNATURE_DESC��ʼ������shader��أ�Ӧ�ÿ���ͨ��Shader�Զ������õ�,����PipelineLayout
*/
//class CU_D3D12_API D3D12GraphicsPipeline : public Pipeline
//{
//public:
//
//	//void bind(InputLayout* layout);
//
//private:
//	// ͨ��VertexLayout����
//	typedef std::map<uint32_t, ID3D12PipelineState*> PSOMap;
//	// ��Ҫ���ж������ͨ��Vertex����
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
