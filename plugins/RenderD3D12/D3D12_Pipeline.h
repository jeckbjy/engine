#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class D3D12_Device;
class D3D12_CommandList;
class D3D12_DescriptorSetLayout;
class CUTE_D3D12_API D3D12_Pipeline : public IPipeline
{
public:
	D3D12_Pipeline();
	~D3D12_Pipeline();

	bool init(D3D12_Device* device, const GraphicsPipelineDesc& desc);
	bool init(D3D12_Device* device, const ComputePipelineDesc& desc);
	void term();

	void issue(ID3D12GraphicsCommandList* cmdList);

private:
	ID3D12PipelineState*		m_pipeline;
	D3D_PRIMITIVE_TOPOLOGY		m_primitive;
	D3D12_DescriptorSetLayout*	m_layout;
	bool						m_isGraphics;
};

//class D3D12_CommandList;
//class CUTE_D3D12_API D3D12_Pipeline : Pipeline
//{
//public:
//	virtual ~D3D12_Pipeline(){}
//
//	virtual void bind(D3D12_CommandList* cmdBuffer) = 0;
//};
//
//class CUTE_D3D12_API D3D12_GraphicsPipeline : public Pipeline
//{
//public:
//	D3D12_GraphicsPipeline(ID3D12Device* device, const PipelineDesc& desc);
//	~D3D12_GraphicsPipeline();
//
//	void bind(D3D12_CommandList* cmdBuffer);
//
//private:
//	typedef std::map<uint32_t, ID3D12PipelineState*> PipelineMap;
//	PipelineMap m_pipelines;
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;
//
//	ID3D12RootSignature* m_rootSignature;
//};
//
//class CUTE_D3D12_API D3D12_ComputePipeline : public Pipeline
//{
//public:
//	D3D12_ComputePipeline(ID3D12Device* device, const PipelineDesc& desc);
//	~D3D12_ComputePipeline();
//
//	void bind(D3D12_CommandList* cmdBuffer);
//
//private:
//	ID3D12PipelineState* m_pipeline;
//};

CUTE_NS_END
