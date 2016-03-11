#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Buffer : public GpuBuffer
{
public:
	D3D12Buffer(ID3D12Device* device, const BUFFER_DESC& desc);
	~D3D12Buffer();

	void* map(size_t offset, size_t len, MAP_FLAG access);
	void unmap();

private:
	ID3D12Resource* m_handle;
	D3D12_RANGE		m_range;
};

CU_NS_END