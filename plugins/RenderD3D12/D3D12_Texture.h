#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_Texture : public ITexture
{
public:
	D3D12_Texture();
	~D3D12_Texture();

	bool init(ID3D12Device* device, const TextureDesc& desc);
	bool init(ID3D12Device* device, ID3D12Resource* resource, RESOURCE_USAGE usage, RESOURCE_STATE state, ComponentMapping components);
	void term();

	void* map() OVERRIDE;
	void  unmap() OVERRIDE;

	bool getSubresourceLayout(SubresourceLayout& layout, uint32 subresource) const OVERRIDE;

	RESOURCE_STATE getState() const { return m_state; }

private:
	//D3D12_Device*		m_device;
	ID3D12Resource*		m_texture;
	RESOURCE_STATE		m_state;
	TextureDesc			m_desc;
};

CUTE_NS_END
