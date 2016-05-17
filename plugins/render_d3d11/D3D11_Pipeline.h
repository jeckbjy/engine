#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Program;
class D3D11CommandBuffer;
class CU_D3D11_API D3D11Pipeline : public Pipeline
{
public:
	virtual void bind(D3D11CommandBuffer* cmdBuffer) = 0;
};

class CU_D3D11_API D3D11GraphicsPipeline : public D3D11Pipeline
{
public:
	D3D11GraphicsPipeline(const GraphicsPipelineDesc& desc);
	~D3D11GraphicsPipeline();

	void bind(D3D11CommandBuffer* cmdBuffer);

private:
	ID3D11RasterizerState*	 m_rasterizer;
	ID3D11BlendState*		 m_blend;
	ID3D11DepthStencilState* m_depthStencil;
	D3D11Program*			 m_vs;
	D3D11Program*			 m_hs;
	D3D11Program*			 m_ds;
	D3D11Program*			 m_gs;
	D3D11Program*			 m_ps;
};

class CU_D3D11_API D3D11ComputePipeline : public D3D11Pipeline
{
public:
	D3D11ComputePipeline(const ComputePipelineDesc& desc);
	~D3D11ComputePipeline();

	void bind(D3D11CommandBuffer* cmdBuffer);

private:
	D3D11Program* m_cs;
};

CU_NS_END