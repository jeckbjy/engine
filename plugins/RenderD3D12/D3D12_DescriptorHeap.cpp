#include "D3D12_DescriptorHeap.h"

CUTE_NS_BEGIN

D3D12_DescriptorHeap::D3D12_DescriptorHeap()
	: m_heap(NULL)
	, m_free(NULL)
	, m_active(NULL)
	, m_size(0)
{
}

D3D12_DescriptorHeap::~D3D12_DescriptorHeap()
{
	term();
}

bool D3D12_DescriptorHeap::init(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
{
	HRESULT hr;

	if (desc.NumDescriptors == 0)
		return true;

	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));
	if (FAILED(hr))
		return false;

	UINT incrementSize = device->GetDescriptorHandleIncrementSize(desc.Type);
	bool hasGPU = (desc.Flags == D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuStart = m_heap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuStart = m_heap->GetGPUDescriptorHandleForHeapStart();

	// create pool
	if (hasGPU)
	{
		for (UINT i = 0; i < desc.NumDescriptors; ++i)
		{
			D3D12_Descriptor* descriptor = new D3D12_Descriptor();
			descriptor->setHandleCPU(cpuStart.ptr + incrementSize * i);
			descriptor->setHandleGPU(gpuStart.ptr + incrementSize * i);
			// 添加到链表中
			descriptor->setNext(m_free);
			m_free = descriptor;
		}
	}
	else
	{
		for (UINT i = 0; i < desc.NumDescriptors; ++i)
		{
			D3D12_Descriptor* descriptor = new D3D12_Descriptor();
			descriptor->setHandleCPU(cpuStart.ptr + incrementSize * i);
			// 添加到链表中
			descriptor->setNext(m_free);
			m_free = descriptor;
		}
	}

	m_size = desc.NumDescriptors;
	return true;
}

void D3D12_DescriptorHeap::term()
{
	D3D12_RELEASE(m_heap);
}

void D3D12_DescriptorHeap::free(D3D12_Descriptor* descriptor)
{
	descriptor->setNext(m_free);
	m_free = descriptor;
}

D3D12_Descriptor* D3D12_DescriptorHeap::create()
{
	return NULL;
}

CUTE_NS_END
