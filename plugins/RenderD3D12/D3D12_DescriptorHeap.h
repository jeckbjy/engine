#pragma once
#include "D3D12_API.h"
#include "D3D12_Descriptor.h"

CUTE_NS_BEGIN

// TODO:如何动态创建Descriptor?
class CUTE_D3D12_API D3D12_DescriptorHeap
{
public:
	D3D12_DescriptorHeap();
	~D3D12_DescriptorHeap();

	bool init(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);
	void term();

	void free(D3D12_Descriptor* descriptor);

	D3D12_Descriptor* create();

	ID3D12DescriptorHeap*	getHeap() const;
	size_t					getSize() const;

private:
	ID3D12DescriptorHeap*	m_heap;
	D3D12_Descriptor*		m_free;
	D3D12_Descriptor*		m_active;
	size_t					m_size;
};

CUTE_NS_END
