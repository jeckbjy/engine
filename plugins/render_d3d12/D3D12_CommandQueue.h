#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12CommondQueue : public CommandQueue
{
public:
	D3D12CommondQueue(ID3D12Device* device);
	~D3D12CommondQueue();

	void submit(CommandBuffer* cmds, Fence* fence);
	void waitIdle();

private:
	ID3D12CommandQueue* m_queue;
};

CU_NS_END