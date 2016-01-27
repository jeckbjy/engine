#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Buffer : public GpuBuffer
{
public:
	D3D12Buffer(const BUFFER_DESC& desc);
	~D3D12Buffer();

	void* map(MAP_FLAG access, size_t offset, size_t len);
	void unmap();

private:
	ID3D12Resource* m_handle;
};

CU_NS_END