#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_API D3D11RasterizerState : public Ref
{
public:
	D3D11RasterizerState(ID3D11DeviceN* device, const RasterizerDesc& desc);
	~D3D11RasterizerState();

	bool equal(const RasterizerDesc& desc) const;

private:
	RasterizerDesc			m_desc;
	ID3D11RasterizerState*	m_state;
};

class CU_API D3D11BlendState : public Ref
{
public:
	D3D11BlendState(const BlendDesc& desc);
	~D3D11BlendState();

private:
	BlendDesc			m_desc;
	ID3D11BlendState*	m_state;
};

class CU_API D3D11DepthStencilState : public Ref
{
public:
	D3D11DepthStencilState(const DepthStencilDesc& desc);
	~D3D11DepthStencilState();

private:
	DepthStencilDesc		 m_desc;
	ID3D11DepthStencilState* m_state;
};

class CU_API D3D11SamplerState : public Ref
{
public:
	D3D11SamplerState(const SamplerDesc& desc);
	~D3D11SamplerState();

private:
	SamplerDesc			m_desc;
	ID3D11SamplerState*	m_state;
};

CU_NS_END
