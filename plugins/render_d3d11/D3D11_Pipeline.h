#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Program;
class CU_D3D11_API D3D11GraphicsPipeline : public Pipeline
{
public:
	D3D11GraphicsPipeline(const GraphicsPipelineDesc& desc);
	~D3D11GraphicsPipeline();

	void bind(ID3D11ContextN* context, const float factors[4], size_t stencilref);

private:
	// ≥÷”–??
	ID3D11RasterizerState*	 m_rasterizer;
	ID3D11BlendState*		 m_blend;
	ID3D11DepthStencilState* m_depthstencil;
};

CU_NS_END