#pragma once
#include "D3D11_API.h"

CUTE_NS_BEGIN

class CUTE_D3D11_API D3D11_RasterizerState : public Ref
{
public:
	D3D11_RasterizerState(ID3D11DeviceN* device, const RasterizerDesc& desc, uint32_t hash);
	~D3D11_RasterizerState();

	bool equal(const RasterizerDesc& desc) const;

	uint32_t getHashCode() const { return m_hash; }
	ID3D11RasterizerState* getState() { return m_state; }

private:
	uint32_t				m_hash;
	RasterizerDesc			m_desc;
	ID3D11RasterizerState*	m_state;
};

class CUTE_D3D11_API D3D11_BlendState : public Ref
{
public:
	D3D11_BlendState(ID3D11DeviceN* device, const BlendDesc& desc, uint32_t hash);
	~D3D11_BlendState();

	bool equal(const BlendDesc& desc) const;

	uint32_t getHashCode() const { return m_hash; }
	ID3D11BlendState* getState() { return m_state; }

private:
	uint32_t			m_hash;
	BlendDesc			m_desc;
	ID3D11BlendState*	m_state;
};

class CUTE_D3D11_API D3D11_DepthStencilState : public Ref
{
public:
	D3D11_DepthStencilState(ID3D11DeviceN* device, const DepthStencilDesc& desc, uint32_t hash);
	~D3D11_DepthStencilState();

	bool equal(const DepthStencilDesc& desc) const;

	uint32_t getHashCode() const { return m_hash; }
	ID3D11DepthStencilState* getState() { return m_state; }

private:
	uint32_t				 m_hash;
	DepthStencilDesc		 m_desc;
	ID3D11DepthStencilState* m_state;
};

class CUTE_D3D11_API D3D11_SamplerState : public Ref
{
public:
	D3D11_SamplerState(ID3D11DeviceN* device, const SamplerDesc& desc, uint32_t hash);
	~D3D11_SamplerState();

	bool equal(const SamplerDesc& desc) const;
	uint32_t getHashCode() const { return m_hash; }

private:
	uint32_t			m_hash;
	SamplerDesc			m_desc;
	ID3D11SamplerState*	m_state;
};

CUTE_NS_END
