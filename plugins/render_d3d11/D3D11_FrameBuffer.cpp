#include "D3D11_FrameBuffer.h"
#include "D3D11_CommandBuffer.h"
#include "D3D11_Texture.h"

CU_NS_BEGIN

D3D11_FrameBuffer::D3D11_FrameBuffer()
	: FrameBuffer()
	, m_dsv(NULL)
{

}

D3D11_FrameBuffer::~D3D11_FrameBuffer()
{

}

void D3D11_FrameBuffer::bind(D3D11_CommandBuffer* cmds)
{
	ID3D11ContextN* context = cmds->getContext();

	if (m_rtv.empty())
		return;
	context->OMSetRenderTargets(m_rtv.size(), &m_rtv[0], m_dsv);
}

void D3D11_FrameBuffer::update()
{
	if (!m_dirty)
		return;
	m_dirty = false;
	if (m_attachments.empty())
		return;

	m_dsv = NULL;
	if (m_attachments[0])
	{
		D3D11_Texture* tex = (D3D11_Texture*)m_attachments[0].get();
		m_dsv = tex->getDSV();
	}

	m_rtv.resize(m_attachments.size() - 1);
	for (size_t i = 1; i < m_attachments.size(); ++i)
	{
		D3D11_Texture* tex = (D3D11_Texture*)m_attachments[i].get();
		m_rtv[i - 1] =  tex ? tex->getRTV() : NULL;
	}
}

CU_NS_END