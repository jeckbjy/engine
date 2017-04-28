#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_Fence : public Fence
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

CUTE_NS_END
