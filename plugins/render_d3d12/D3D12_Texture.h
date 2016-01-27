#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Texture : public Texture
{
public:
	D3D12Texture(const TEXTURE_DESC& desc);
	~D3D12Texture();

	void* map(PixelData& data, MAP_FLAG flag, uint level = 0, uint face = 0);
	void unmap();

private:
	ID3D12Resource* m_handle;
};

CU_NS_END