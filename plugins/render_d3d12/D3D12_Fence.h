#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_Fence : public Fence
{
public:
	D3D12_Fence(ID3D12Device* device);
	~D3D12_Fence();

	void signal(ID3D12CommandQueue* queue);

	virtual void wait();
	virtual void wait(uint64_t timeout);

private:
	ID3D12Fence*	m_fence;
	HANDLE			m_event;
	uint			m_value;
};

CU_NS_END