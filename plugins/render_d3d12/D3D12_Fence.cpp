#include "D3D12_Fence.h"

CU_NS_BEGIN

D3D12_Fence::D3D12_Fence(ID3D12Device* device)
	: m_fence(NULL)
	, m_event(INVALID_HANDLE_VALUE)
	, m_value(1)
{
	m_event = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (!m_event)
		return;
	HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	D3D12_CHECK(hr, "CreateFence failed!");
}

D3D12_Fence::~D3D12_Fence()
{
	D3D12_RELEASE(m_fence);
}

void D3D12_Fence::signal(ID3D12CommandQueue* queue)
{
	queue->Signal(m_fence, m_value);
	++m_value;
}

void D3D12_Fence::wait()
{
	UINT64 fenceToWaitFor = m_value - 1;
	if (m_fence->GetCompletedValue() < fenceToWaitFor) {
		HRESULT hr = m_fence->SetEventOnCompletion(fenceToWaitFor, m_event);
		if (FAILED(hr)) {
			//logger.error(LOG_GRAPHICS, "Direct3D12Fence::wait: SetEventOnCompletion failed");
			return;
		}
		WaitForSingleObject(m_event, INFINITE);
	}
}

void D3D12_Fence::wait(uint64_t timeout)
{
	UINT64 fenceToWaitFor = m_value - 1;
	if (m_fence->GetCompletedValue() < fenceToWaitFor) {
		HRESULT hr = m_fence->SetEventOnCompletion(fenceToWaitFor, m_event);
		if (FAILED(hr)) {
			//logger.error(LOG_GRAPHICS, "Direct3D12Fence::wait: SetEventOnCompletion failed");
			return;
		}
		WaitForSingleObject(m_event, timeout);
	}
}

CU_NS_END