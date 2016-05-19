#pragma once
#include "D3D11_API.h"
#include "SmartPtr.h"

CU_NS_BEGIN

class D3D11Device;
class D3D11Program;
class D3D11CommandBuffer;

class D3D11BlendState;
class D3D11RasterizerState;
class D3D11DepthStencilState;

class CU_D3D11_API D3D11Pipeline : public Pipeline
{
public:
	virtual void bind(D3D11CommandBuffer* cmdBuffer) = 0;
};

class CU_D3D11_API D3D11GraphicsPipeline : public D3D11Pipeline
{
public:
	D3D11GraphicsPipeline(D3D11Device* device, const GraphicsPipelineDesc& desc);
	~D3D11GraphicsPipeline();

	void bind(D3D11CommandBuffer* cmdBuffer);

private:
	typedef SharedPtr<D3D11Program>		ProgramPtr;
	typedef ComPtr<ID3D11InputLayout>	ILayoutPtr;

	D3D11BlendState*		m_blend;
	D3D11RasterizerState*	m_rasterizer;
	D3D11DepthStencilState*	m_depthStencil;

	// 当前使用
	InputLayout*			m_curLayout;
	ILayoutPtr				m_d3dLayout;

	ProgramPtr				m_vs;
	ProgramPtr				m_hs;
	ProgramPtr				m_ds;
	ProgramPtr				m_gs;
	ProgramPtr				m_ps;
};

class CU_D3D11_API D3D11ComputePipeline : public D3D11Pipeline
{
public:
	D3D11ComputePipeline(const ComputePipelineDesc& desc);
	~D3D11ComputePipeline();

	void bind(D3D11CommandBuffer* cmdBuffer);

private:
	typedef SharedPtr<D3D11Program> ProgramPtr;
	ProgramPtr m_cs;
};

CU_NS_END