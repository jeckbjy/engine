#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_CommondQueue : public CommandQueue
{
public:
	D3D12_CommondQueue(ID3D12Device* device);
	~D3D12_CommondQueue();

	void submit(CommandBuffer* cmds, Fence* fence);
	void waitIdle();

private:
	ID3D12CommandQueue* m_queue;
};

CUTE_NS_END
