#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class D3D11Program;
class CU_D3D11_API D3D11Pipeline : public Pipeline
{
public:
	D3D11Pipeline(const PIPELINE_DESC& desc);
	~D3D11Pipeline();

	void bind(ID3D11DeviceContextN* context, const float factors[4], size_t stencilref);

private:
	D3D11Program*				m_prog;
	ID3D11DepthStencilState*	_depth_stencil;
	ID3D11BlendState*			_blend;
	ID3D11RasterizerState*		_rasterizer;
	ID3D11SamplerState*			_sampler;		// ¶à¸ö
	uint32_t					_mask;			// ÑÚÂë
};

CU_NS_END