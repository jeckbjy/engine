#include "D3D12_Buffer.h"

CU_NS_BEGIN

D3D12Buffer::D3D12Buffer(ID3D12Device* device, const BUFFER_DESC& desc)
	: GpuBuffer(desc)
	, m_handle(NULL)
{
	D3D12_RESOURCE_DESC dx_desc;
	dx_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	dx_desc.Alignment = 0;
	dx_desc.Width = m_bytes;
	dx_desc.Height = 1;
	dx_desc.DepthOrArraySize = 1;
	dx_desc.MipLevels = 0;
	dx_desc.Format = DXGI_FORMAT_UNKNOWN;
	dx_desc.SampleDesc.Count = 1;
	dx_desc.SampleDesc.Quality = 0;
	dx_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	dx_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES props;
	props.Type = D3D12_HEAP_TYPE_UPLOAD;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	props.CreationNodeMask = 1;
	props.VisibleNodeMask = 1;
	device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &dx_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&m_handle));
}

D3D12Buffer::~D3D12Buffer()
{
	D3D12_RELEASE(m_handle);
}

void* D3D12Buffer::map(size_t offset, size_t len, MAP_FLAG access)
{
	CU_UNUSED(access);
	void* data = NULL;
	m_range.Begin = offset;
	m_range.End = offset + len;
	m_handle->Map(0, &m_range, &data);
	return data;
}

void D3D12Buffer::unmap()
{
	m_handle->Unmap(0, &m_range);
}

CU_NS_END