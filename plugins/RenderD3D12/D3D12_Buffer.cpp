#include "D3D12_Buffer.h"
#include "D3D12_Mapping.h"

CUTE_NS_BEGIN

D3D12_Bufffer::D3D12_Bufffer()
	: m_buffer(NULL)
{
}

D3D12_Bufffer::~D3D12_Bufffer()
{
	term();
}

bool D3D12_Bufffer::init(ID3D12Device* device, const BufferDesc& desc)
{
	D3D12_HEAP_PROPERTIES prop = {};
    prop.Type                   = D3D12_Mapping::getHeapType(desc.heapProperty.type);
    prop.CPUPageProperty        = D3D12_Mapping::getCpuPageProperty(desc.heapProperty.cpuPageProperty);
    prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
    prop.VisibleNodeMask        = 1;
    prop.CreationNodeMask       = 1;

    D3D12_RESOURCE_DESC info = {};
	info.Format					= DXGI_FORMAT_UNKNOWN;
    info.Dimension				= D3D12_RESOURCE_DIMENSION_BUFFER;
    info.Width					= desc.size;
    info.Height					= 1;
    info.DepthOrArraySize		= 1;
    info.MipLevels				= 1;
    info.SampleDesc.Count		= 1;
    info.SampleDesc.Quality		= 0;
    info.Layout					= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    info.Flags					= D3D12_Mapping::getResourceFlags(desc.usage);

	D3D12_RESOURCE_STATES   state = D3D12_Mapping::getResourceStates(desc.state);
    D3D12_HEAP_FLAGS        flags = D3D12_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateCommittedResource(&prop, flags, &info, state, NULL, IID_PPV_ARGS(&m_buffer));
	if (FAILED(hr))
		return false;

	memcpy(&m_desc, &desc, sizeof(m_desc));
	m_state = desc.state;
	return true;
}

void D3D12_Bufffer::term()
{
	if (m_buffer == NULL)
		return;

	D3D12_RELEASE(m_buffer);
}

void* D3D12_Bufffer::map()
{
	HRESULT hr;
	void* ptr = NULL;

	if (m_desc.heapProperty.type == HEAP_TYPE_READBACK)
	{
		D3D12_RANGE range = {};
		range.Begin = 0;
		range.End = SIZE_T(m_desc.size);

		hr = m_buffer->Map(0, &range, &ptr);
	}
	else
	{
		hr = m_buffer->Map(0, NULL, &ptr);
	}

	if (FAILED(hr))
		return NULL;

	return ptr;
}

void D3D12_Bufffer::unmap()
{
	if (m_desc.heapProperty.type == HEAP_TYPE_READBACK)
	{
		m_buffer->Unmap(0, NULL);
	}
	else
	{
		D3D12_RANGE range = {};
		range.Begin = 0;
		range.End = SIZE_T(m_desc.size);
		m_buffer->Unmap(0, &range);
	}
}

//D3D12_Buffer::D3D12_Buffer(ID3D12Device* device, const BufferDesc& desc)
//	: GpuBuffer(desc)
//	, m_handle(NULL)
//{
//	D3D12_RESOURCE_DESC dx_desc;
//	dx_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	dx_desc.Alignment = 0;
//	dx_desc.Width = m_bytes;
//	dx_desc.Height = 1;
//	dx_desc.DepthOrArraySize = 1;
//	dx_desc.MipLevels = 0;
//	dx_desc.Format = DXGI_FORMAT_UNKNOWN;
//	dx_desc.SampleDesc.Count = 1;
//	dx_desc.SampleDesc.Quality = 0;
//	dx_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
//	dx_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
//
//	D3D12_HEAP_PROPERTIES props;
//	props.Type = D3D12_HEAP_TYPE_UPLOAD;
//	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//	props.CreationNodeMask = 1;
//	props.VisibleNodeMask = 1;
//	device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &dx_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&m_handle));
//}
//
//D3D12_Buffer::~D3D12_Buffer()
//{
//	D3D12_RELEASE(m_handle);
//}
//
//void* D3D12_Buffer::map(size_t offset, size_t len, MAP_FLAG access)
//{
//	CU_UNUSED(access);
//	void* data = NULL;
//	D3D12_RANGE range;
//	range.Begin = offset;
//	range.End = offset + len;
//	m_handle->Map(0, &range, &data);
//	m_range = range;
//	return data;
//}
//
//void D3D12_Buffer::unmap()
//{
//	m_handle->Unmap(0, &m_range);
//}

CUTE_NS_END
