#pragma once
#include "D3D12_API.h"
#include "Cute/Mutex.h"
#include "Cute/DynamicArray.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_CommandQueue : public ICommandQueue
{
public:
	D3D12_CommandQueue();
	~D3D12_CommandQueue();

	bool init(ID3D12Device* device, COMMANDLIST_TYPE type, size_t maxSubmitCount);
	void term();

	bool submit(ICommandList* cmdList) OVERRIDE;
	void execute(IFence* fence) OVERRIDE;
	void waitIdle() OVERRIDE;

	ID3D12CommandQueue* getQueue() const { return m_queue; }

private:
	typedef DynamicArray<ID3D12CommandList*> SubmitArray;
	ID3D12CommandQueue* m_queue;
	ID3D12Fence*		m_fence;
	HANDLE				m_event;
	SubmitArray			m_submits;
	size_t				m_submitIndex;
	Mutex				m_mutex;
};

CUTE_NS_END
