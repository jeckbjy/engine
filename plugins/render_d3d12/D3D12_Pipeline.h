#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Pipeline : public Pipeline
{
public:
	D3D12Pipeline(ID3D12Device* device, const GRAPHICS_PIPELINE_DESC& desc);
	D3D12Pipeline(ID3D12Device* device, const COMPUTE_PIPELINE_DESC& desc);
	~D3D12Pipeline();

private:
	ID3D12PipelineState* m_pipeline;
};

CU_NS_END
