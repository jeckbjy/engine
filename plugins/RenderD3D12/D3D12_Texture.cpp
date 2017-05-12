#include "D3D12_Texture.h"
#include "D3D12_Mapping.h"

CUTE_NS_BEGIN

D3D12_Texture::D3D12_Texture()
	: m_texture(NULL)
	, m_state(RESOURCE_STATE_UNKNOWN)
{
}

D3D12_Texture::~D3D12_Texture()
{
	term();
}

bool D3D12_Texture::init(ID3D12Device* device, const TextureDesc& desc)
{
	DXGI_FORMAT				format = D3D12_Mapping::getFormat(desc.format);
	D3D12_TEXTURE_LAYOUT	layout = desc.layout == RESOURCE_LAYOUT_LINEAR ? D3D12_TEXTURE_LAYOUT_ROW_MAJOR : D3D12_TEXTURE_LAYOUT_UNKNOWN;
	D3D12_RESOURCE_STATES	state  = D3D12_Mapping::getResourceStates(desc.initState);
	D3D12_HEAP_FLAGS		flags  = D3D12_HEAP_FLAG_NONE;

	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_Mapping::getHeapType(desc.heapProperty.type);
	prop.CPUPageProperty		= D3D12_Mapping::getCpuPageProperty(desc.heapProperty.cpuPageProperty);
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.VisibleNodeMask		= 1;
	prop.CreationNodeMask		= 1;

	D3D12_RESOURCE_DESC info = {};
	info.Dimension				= D3D12_Mapping::getResorceDimension(desc.dimension);
	info.Width					= desc.width;
	info.Height					= desc.height;
	info.DepthOrArraySize		= desc.depthOrArraySize;
	info.Format					= format;
	info.MipLevels				= desc.mipLevels;
	info.SampleDesc.Count		= desc.sampleCount;
	info.SampleDesc.Quality		= 0;
	info.Layout					= layout;
	info.Flags					= D3D12_Mapping::getResourceFlags(desc.usage);

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format;

	bool isTarget = false;
	if (desc.usage & RESOURCE_USAGE_DEPTH_TARGET)
	{
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;
		isTarget = true;
	}

	if (desc.usage & RESOURCE_USAGE_COLOR_TARGET)
	{
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 0.0f;
		isTarget = true;
	}

	HRESULT hr = device->CreateCommittedResource(
		&prop, flags, &info, state, (isTarget ? &clearValue : NULL), IID_PPV_ARGS(&m_texture));
	if (FAILED(hr))
		return false;

	memcpy(&m_desc, &desc, sizeof(m_desc));
	m_state = desc.initState;

	return true;
}

bool D3D12_Texture::init(ID3D12Device* device, ID3D12Resource* resource, RESOURCE_USAGE usage, RESOURCE_STATE state, ComponentMapping components)
{
	m_texture = resource;
	m_state = state;

	// ÉèÖÃdesc
	return true;
}

void D3D12_Texture::term()
{
	D3D12_RELEASE(m_texture);
	m_state = RESOURCE_STATE_UNKNOWN;
}

void* D3D12_Texture::map()
{
	void* ptr = NULL;
	HRESULT hr = m_texture->Map(0, NULL, &ptr);
	if (FAILED(hr))
		return NULL;

	return ptr;
}

void D3D12_Texture::unmap()
{
	m_texture->Unmap(0, NULL);
}

bool D3D12_Texture::getSubresourceLayout(SubresourceLayout& result, uint32 subresource) const
{
	ID3D12Device* device = NULL;
	HRESULT hr = m_texture->GetDevice(__uuidof(ID3D12Device), reinterpret_cast<void**>(&device));
	if (FAILED(hr))
		return false;

	D3D12_RESOURCE_DESC nativeDesc = m_texture->GetDesc();

	uint32_t rowCount = 0;
	uint64_t rowPitch = 0;
	uint64_t size = 0;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout = {};
	device->GetCopyableFootprints(
		&nativeDesc,
		subresource,
		1,
		0,
		&layout,
		&rowCount,
		&rowPitch,
		&size);

	result.offset		= layout.Offset;
	result.rowCount		= rowCount;
	result.rowPitch		= rowPitch;
	result.slicePitch	= layout.Footprint.RowPitch * layout.Footprint.Height;
	result.size			= size;

	return true;
}

CUTE_NS_END
