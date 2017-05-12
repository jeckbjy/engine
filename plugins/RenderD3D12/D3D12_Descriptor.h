#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_Descriptor
{
public:
	D3D12_Descriptor():m_next(NULL){}
	~D3D12_Descriptor(){}

	void setHandleCPU(SIZE_T ptr);
	void setHandleGPU(UINT64 ptr);
	void setNext(D3D12_Descriptor* next);

	const D3D12_CPU_DESCRIPTOR_HANDLE& getHandleCPU() const;
	const D3D12_GPU_DESCRIPTOR_HANDLE& getHandleGPU() const;

public:
	friend class D3D12_DescriptorHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE m_cpu;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpu;
	D3D12_Descriptor*			m_next;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void D3D12_Descriptor::setHandleCPU(SIZE_T ptr)
{
	m_cpu.ptr = ptr;
}

inline void D3D12_Descriptor::setHandleGPU(UINT64 ptr)
{
	m_gpu.ptr = ptr;
}

inline void D3D12_Descriptor::setNext(D3D12_Descriptor* next)
{
	m_next = next;
}

inline const D3D12_CPU_DESCRIPTOR_HANDLE& D3D12_Descriptor::getHandleCPU() const
{
	return m_cpu;
}

inline const D3D12_GPU_DESCRIPTOR_HANDLE& D3D12_Descriptor::getHandleGPU() const
{
	return m_gpu;
}

CUTE_NS_END
