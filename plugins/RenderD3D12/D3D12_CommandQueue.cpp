#include "D3D12_CommandQueue.h"
//#include "D3D12_CommandBuffer.h"
#include "D3D12_Mapping.h"
#include "D3D12_Fence.h"

CUTE_NS_BEGIN

D3D12_CommandQueue::D3D12_CommandQueue()
	: m_queue(NULL)
	, m_fence(NULL)
	, m_event(INVALID_HANDLE_VALUE)
{
}

D3D12_CommandQueue::~D3D12_CommandQueue()
{
	term();
}

bool D3D12_CommandQueue::init(ID3D12Device* device, COMMANDLIST_TYPE type, size_t maxSubmitCount)
{
	HRESULT hr;

	D3D12_COMMAND_QUEUE_DESC info = {};
	info.Type		= D3D12_Mapping::getCommandListType(type);
	info.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	info.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	info.NodeMask	= 0;

	hr = device->CreateCommandQueue(&info, IID_PPV_ARGS(&m_queue));
	if (FAILED(hr))
		return false;

	m_submitIndex = 0;
	m_submits.resize(maxSubmitCount);

	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr))
		return false;

	m_event = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (m_event == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

void D3D12_CommandQueue::term()
{
	if (m_queue != NULL && m_fence != NULL && m_event != INVALID_HANDLE_VALUE)
	{
		waitIdle();
	}

	m_submits.clear();
	m_submitIndex = 0;

	D3D12_RELEASE(m_queue);
	D3D12_RELEASE(m_fence);

	if (m_event != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_event);
		m_event = INVALID_HANDLE_VALUE;
	}
}

bool D3D12_CommandQueue::submit(ICommandList* cmdList)
{
	Mutex::ScopedLock guard(m_mutex);

	if (m_submitIndex + 1 >= m_submits.size())
		return false;

	return true;
}

void D3D12_CommandQueue::execute(IFence* fence)
{
	m_queue->ExecuteCommandLists(m_submitIndex, m_submits.data());
	m_submitIndex = 0;

	if (fence != NULL)
	{
		D3D12_Fence* pWrapFence = static_cast<D3D12_Fence*>(fence);
		ID3D12Fence * pNativeFence = pWrapFence->getFence();
		UINT64 value = pWrapFence->getValue();
		m_queue->Signal(pNativeFence, value);
		pWrapFence->advanceValue();
	}
}

void D3D12_CommandQueue::waitIdle()
{
	m_fence->Signal(0);
	m_fence->SetEventOnCompletion(1, m_event);
	m_queue->Signal(m_fence, 1);
	WaitForSingleObject(m_event, INFINITE);
}

CUTE_NS_END
