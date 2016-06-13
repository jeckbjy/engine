#include "D3D12_FrameBuffer.h"

CU_NS_BEGIN

D3D12_FrameBuffer::D3D12_FrameBuffer()
{

}

D3D12_FrameBuffer::~D3D12_FrameBuffer()
{

}

void D3D12_FrameBuffer::bind(ID3D12GraphicsCommandList* cmdList)
{
	update();
	cmdList->OMSetRenderTargets(m_attachments.size() - 1, m_rtv.GetCPUDescriptorHandleForHeapStart(), true, m_dsv);
}

void D3D12_FrameBuffer::update()
{
	if (!m_dirty)
		return;

	m_dirty = false;

	// ´´½¨rtv
}

CU_NS_END