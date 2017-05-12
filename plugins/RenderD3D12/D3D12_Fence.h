#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_Fence : public IFence
{
public:
	D3D12_Fence();
	~D3D12_Fence();

	bool init(ID3D12Device* device);
	void term();

	bool isSignaled() const OVERRIDE;
	bool wait(uint32 msec) OVERRIDE;

	ID3D12Fence* getFence() const { return m_fence; }
	HANDLE		 getEvent() const { return m_event; }
	UINT64		 getValue() const { return m_currValue; }
	void		 advanceValue();

private:
	ID3D12Fence*	m_fence;
	HANDLE			m_event;
	UINT32			m_prevValue;
	UINT32			m_currValue;
};

CUTE_NS_END
