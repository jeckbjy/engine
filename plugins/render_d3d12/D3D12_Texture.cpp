#include "D3D12_Texture.h"

CU_NS_BEGIN

D3D12Texture::D3D12Texture(const TEXTURE_DESC& desc)
:Texture(desc)
{

}

D3D12Texture::~D3D12Texture()
{
	D3D12_RELEASE(m_handle);
}

void* D3D12Texture::map(PixelData& data, MAP_FLAG flag, uint level /* = 0 */, uint face /* = 0 */)
{
	return 0;
}

void D3D12Texture::unmap()
{
	m_handle->Unmap(0, 0);
}

CU_NS_END