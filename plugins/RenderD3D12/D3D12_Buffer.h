#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_Bufffer : public IBuffer
{
public:
	D3D12_Bufffer();
	~D3D12_Bufffer();

	bool init(ID3D12Device* device, const BufferDesc& desc);
	void term();

	void* map();
	void  unmap();

	ID3D12Resource*				getBuffer() const { return m_buffer; }
	D3D12_GPU_VIRTUAL_ADDRESS	getGPUVirtualAddress() const { return m_buffer->GetGPUVirtualAddress(); }

private:
	ID3D12Resource* m_buffer;
	RESOURCE_STATE	m_state;
	BufferDesc		m_desc;
};

//class CUTE_D3D12_API D3D12_Buffer : public GpuBuffer
//{
//public:
//	D3D12_Buffer(ID3D12Device* device, const BufferDesc& desc);
//	~D3D12_Buffer();
//
//	void* map(size_t offset, size_t len, MAP_FLAG access);
//	void unmap();
//
//	D3D12_GPU_VIRTUAL_ADDRESS getGPUVirtualAddress() { return m_handle->GetGPUVirtualAddress();  }
//
//private:
//	ID3D12Resource* m_handle;
//	D3D12_RANGE		m_range;
//};

CUTE_NS_END
