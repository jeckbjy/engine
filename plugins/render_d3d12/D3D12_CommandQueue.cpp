#include "D3D12_CommandQueue.h"
#include "D3D12_CommandBuffer.h"

CU_NS_BEGIN

D3D12CommondQueue::D3D12CommondQueue(ID3D12Device* device)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	D3D12_CHECK(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue)),
		"D3D12 CreateCommandQueue Fail!");
}

D3D12CommondQueue::~D3D12CommondQueue()
{
	D3D12_RELEASE(m_queue);
}

void D3D12CommondQueue::submit(CommandBuffer* cmds, Fence* fence)
{
	D3D12CommandBuffer* dx_cmds = (D3D12CommandBuffer*)cmds;
	ID3D12CommandList* cmd_list[] = { dx_cmds->native() };
	m_queue->ExecuteCommandLists(1, cmd_list);

	//if (fence)
}

void D3D12CommondQueue::waitIdle()
{

}

CU_NS_END
