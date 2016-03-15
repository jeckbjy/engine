#include "D3D11_Texture.h"

CU_NS_BEGIN

D3D11Texture::D3D11Texture(const TextureDesc& desc)
:Texture(desc), _handle(NULL)
{

}

D3D11Texture::~D3D11Texture()
{

}

CU_NS_END