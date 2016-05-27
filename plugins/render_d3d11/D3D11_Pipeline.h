#pragma once
#include "D3D11_API.h"
#include "SmartPtr.h"

CU_NS_BEGIN

class D3D11_Device;
class D3D11_Shader;
class D3D11_Program;
class D3D11_InputLayout;
class D3D11_CommandBuffer;

class D3D11_BlendState;
class D3D11_RasterizerState;
class D3D11_DepthStencilState;

class CU_D3D11_API D3D11_Pipeline : public Pipeline
{
public:
	D3D11_Pipeline(D3D11_Device* device, const PipelineDesc& desc);
	~D3D11_Pipeline();

	void bind(D3D11_CommandBuffer* cmdBuffer);

	D3D11_Program* getProgram() { return m_program; }

private:
	typedef ComPtr<ID3D11InputLayout> ComInputLayout;
	D3D11_Program*	m_program;
	D3D11_BlendState*		m_blend;
	D3D11_RasterizerState*	m_rasterizer;
	D3D11_DepthStencilState*	m_depthStencil;
	D3D11_InputLayout*		m_curlayout;
	ComInputLayout			m_d3dLayout;
};

CU_NS_END