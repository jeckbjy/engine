#include "D3D12_Fence.h"

CUTE_NS_BEGIN

D3D12_Fence::D3D12_Fence()
	: m_fence(NULL)
	, m_event(INVALID_HANDLE_VALUE)
	, m_prevValue(0)
	, m_currValue(0)
{
}

D3D12_Fence::~D3D12_Fence()
{
	term();
}

bool D3D12_Fence::init(ID3D12Device* device)
{
	m_prevValue = 0;
	m_currValue = 0;
	HRESULT hr = device->CreateFence(m_currValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr))
		return false;

	m_event = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (m_event == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

void D3D12_Fence::term()
{
	D3D12_RELEASE(m_fence);
	if (m_event != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_event);
		m_event = INVALID_HANDLE_VALUE;
	}
}

bool D3D12_Fence::isSignaled() const
{
	return m_fence->GetCompletedValue() >= m_prevValue;
}

bool D3D12_Fence::wait(uint32 msec)
{
	HRESULT hr;
	if (m_fence->GetCompletedValue() < m_prevValue)
	{
		hr = m_fence->SetEventOnCompletion(m_prevValue, m_event);
		if (FAILED(hr))
			return false;

		if (WaitForSingleObjectEx(m_event, msec, FALSE) != WAIT_OBJECT_0)
			return false;
	}

	return true;
}

void D3D12_Fence::advanceValue()
{
	m_prevValue = m_currValue;
	m_currValue++;
}

CUTE_NS_END
