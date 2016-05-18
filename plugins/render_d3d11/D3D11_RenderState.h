#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11RasterizerState : public Ref
{
public:
	D3D11RasterizerState(ID3D11DeviceN* device, const RasterizerDesc& desc, uint32_t hash);
	~D3D11RasterizerState();

	bool equal(const RasterizerDesc& desc) const;

private:
	uint32_t				m_hash;
	RasterizerDesc			m_desc;
	ID3D11RasterizerState*	m_state;
};

class CU_D3D11_API D3D11BlendState : public Ref
{
public:
	D3D11BlendState(ID3D11DeviceN* device, const BlendDesc& desc, uint32_t hash);
	~D3D11BlendState();

	bool equal(const BlendDesc& desc) const;

private:
	uint32_t			m_hash;
	BlendDesc			m_desc;
	ID3D11BlendState*	m_state;
};

class CU_D3D11_API D3D11DepthStencilState : public Ref
{
public:
	D3D11DepthStencilState(ID3D11DeviceN* device, const DepthStencilDesc& desc, uint32_t hash);
	~D3D11DepthStencilState();

	bool equal(const DepthStencilDesc& desc) const;

private:
	uint32_t				 m_hash;
	DepthStencilDesc		 m_desc;
	ID3D11DepthStencilState* m_state;
};

class CU_D3D11_API D3D11SamplerState : public Ref
{
public:
	D3D11SamplerState(ID3D11DeviceN* device, const SamplerDesc& desc, uint32_t hash);
	~D3D11SamplerState();

	bool equal(const SamplerDesc& desc) const;

private:
	uint32_t			m_hash;
	SamplerDesc			m_desc;
	ID3D11SamplerState*	m_state;
};

CU_NS_END