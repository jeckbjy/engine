#include "D3D12_FrameBuffer.h"

CUTE_NS_BEGIN

D3D12_FrameBuffer::D3D12_FrameBuffer()
{

}

D3D12_FrameBuffer::~D3D12_FrameBuffer()
{

}

void D3D12_FrameBuffer::bind(void* param)
{
	ID3D12GraphicsCommandList* cmdList = (ID3D12GraphicsCommandList*)param;
	update();
	//cmdList->OMSetRenderTargets(m_attachments.size() - 1, m_rtv->GetCPUDescriptorHandleForHeapStart(), true, m_dsv);
}

void D3D12_FrameBuffer::update()
{
	//if (!m_dirty)
	//	return;

	//m_dirty = false;

	//// ´´½¨rtv
}

CUTE_NS_END
