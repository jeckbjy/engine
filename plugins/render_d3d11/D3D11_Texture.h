#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_Texture : public Texture
{
public:
	D3D11_Texture(const TextureDesc& desc, ID3D11DeviceN* device);
	~D3D11_Texture();

	void* map(PixelData& data, MAP_FLAG flag, uint mipLevel = 0, uint face = 0);
	void unmap();

	ID3D11DepthStencilView* getDSV() { return (ID3D11DepthStencilView*)m_view; }

private:
	void create1D(ID3D11DeviceN* device, const TextureDesc& desc);
	void create2D(ID3D11DeviceN* device, const TextureDesc& desc);
	void create3D(ID3D11DeviceN* device, const TextureDesc& desc);

private:
	ID3D11Resource* m_handle;
	ID3D11View*		m_view;
	UINT			m_lockedSubresourceIdx;
};

CU_NS_END