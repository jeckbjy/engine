#include "D3D12_Buffer.h"

CU_NS_BEGIN

D3D12Buffer::D3D12Buffer(const BUFFER_DESC& desc)
:GpuBuffer(desc)
{

}

D3D12Buffer::~D3D12Buffer()
{
	D3D12_RELEASE(m_handle);
}

void* D3D12Buffer::map(MAP_FLAG access, size_t offset, size_t len)
{
	void* data = NULL;
	m_handle->Map(offset, 0, &data);
	return data;
}

void D3D12Buffer::unmap()
{
	m_handle->Unmap(0, NULL);
}

CU_NS_END