#include "D3D11_Texture.h"
#include "D3D11_Mapping.h"
#include "D3D11_Device.h"

CU_NS_BEGIN

UINT getBindFlags(TextureUsage usage)
{
	if (usage == TU_DEPTH_STENCIL)
		return D3D11_BIND_DEPTH_STENCIL;
	return 0;
}

D3D11Texture::D3D11Texture(const TextureDesc& desc, ID3D11DeviceN* device)
	: Texture(desc)
	, m_handle(NULL)
	, m_view(NULL)
{
	switch (desc.type)
	{
	case TEX_1D:
		create1D(device, desc);
		break;
	case TEX_2D:
	case TEX_CUBE:
		create2D(device, desc);
		break;
	case TEX_3D:
		create3D(device, desc);
		break;
	default:
		break;
	}
}

D3D11Texture::~D3D11Texture()
{
	D3D11_RELEASE(m_view);
	D3D11_RELEASE(m_handle);
}

void D3D11Texture::create1D(ID3D11DeviceN* device, const TextureDesc& desc)
{
	D3D11_TEXTURE1D_DESC dx_desc;
	dx_desc.Width = desc.width;
	dx_desc.MipLevels = desc.mipLevels;
	dx_desc.ArraySize = desc.depthOrArraySize;
	dx_desc.Format = D3D11Mapping::getFormat(desc.format);

	dx_desc.Usage = D3D11_USAGE_DEFAULT;
	dx_desc.BindFlags = getBindFlags(desc.usage);
	dx_desc.CPUAccessFlags = 0;
	dx_desc.MiscFlags = 0;

	device->CreateTexture1D(&dx_desc, NULL,(ID3D11Texture1D**)&m_handle);
}

void D3D11Texture::create2D(ID3D11DeviceN* device, const TextureDesc& desc)
{
	D3D11_TEXTURE2D_DESC dx_desc;
	dx_desc.Width = desc.width;
	dx_desc.Height = desc.height;
	dx_desc.MipLevels = desc.mipLevels;
	dx_desc.Format = D3D11Mapping::getFormat(desc.format);

	dx_desc.Usage = D3D11_USAGE_DEFAULT;
	dx_desc.BindFlags = getBindFlags(desc.usage);
	dx_desc.CPUAccessFlags = 0;
	dx_desc.MiscFlags = 0;

	if (desc.type == TEX_CUBE)
	{
		dx_desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		dx_desc.ArraySize = desc.depthOrArraySize * 6;
	}
	else
	{
		dx_desc.ArraySize = desc.depthOrArraySize;
	}

	device->CreateTexture2D(&dx_desc, NULL, (ID3D11Texture2D**)&m_handle);
}

void D3D11Texture::create3D(ID3D11DeviceN* device, const TextureDesc& desc)
{
	D3D11_TEXTURE3D_DESC dx_desc;
	dx_desc.Width = desc.width;
	dx_desc.Height = desc.height;
	dx_desc.Depth = desc.depthOrArraySize;
	dx_desc.MiscFlags = desc.mipLevels;
	dx_desc.Format = D3D11Mapping::getFormat(desc.format);

	dx_desc.Usage = D3D11_USAGE_DEFAULT;
	dx_desc.BindFlags = getBindFlags(desc.usage);
	dx_desc.CPUAccessFlags = 0;
	dx_desc.MiscFlags = 0;

	device->CreateTexture3D(&dx_desc, NULL, (ID3D11Texture3D**)&m_handle);
}

void* D3D11Texture::map(PixelData& data, MAP_FLAG flag, uint mipLevel /* = 0 */, uint face /* = 0 */)
{
	ID3D11ContextN* context = gD3D11Context();
	if (!context)
		return NULL;

	D3D11_MAP type;
	D3D11_MAPPED_SUBRESOURCE res;
	m_lockedSubresourceIdx = D3D11CalcSubresource(mipLevel, face, m_mipmaps);
	context->Map(m_handle, m_lockedSubresourceIdx, type, 0, &res);
	return (char*)res.pData;
}

void D3D11Texture::unmap()
{
	ID3D11ContextN* context = gD3D11Context();
	if (context)
		context->Unmap(m_handle, m_lockedSubresourceIdx);
}

CU_NS_END
