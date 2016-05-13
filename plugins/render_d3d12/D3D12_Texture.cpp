#include "D3D12_Texture.h"
#include "D3D12_Mapping.h"

CU_NS_BEGIN

D3D12Texture::D3D12Texture(ID3D12Device* device, const TextureDesc& desc)
	: Texture(desc)
	, m_handle(NULL)
{
	D3D12_RESOURCE_DESC dx_desc;
	dx_desc.Dimension = D3D12Mapping::getTextureType(desc.type);
	dx_desc.Alignment = 0;
	dx_desc.Width = desc.width;
	dx_desc.Height = desc.height;
	dx_desc.DepthOrArraySize = desc.depthOrArraySize;
	dx_desc.MipLevels = desc.mipLevels;
	dx_desc.Format = D3D12Mapping::getFormat(desc.format);
	dx_desc.SampleDesc.Count = desc.samples;
	dx_desc.SampleDesc.Quality = desc.quality;
	dx_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	dx_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_HEAP_PROPERTIES props;
	props.Type = D3D12_HEAP_TYPE_UPLOAD;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	props.CreationNodeMask = 1;
	props.VisibleNodeMask = 1;
	device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &dx_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&m_handle));
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