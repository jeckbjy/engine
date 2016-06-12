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

	if (m_dirty)
	{
		update();
	}
	if (m_rtv.empty())
		return;
	context->OMSetRenderTargets(m_rtv.size(), &m_rtv[0], m_dsv);
}

void D3D11_FrameBuffer::update()
{
	m_dirty = false;
	if (m_colors.empty())
		return;

	m_dsv = NULL;
	if (m_depthStencil)
	{
		D3D11_Texture* tex = (D3D11_Texture*)m_depthStencil.get();
		m_dsv = tex->getDSV();
	}

	m_rtv.resize(m_colors.size());
	for (size_t i = 0; i < m_colors.size(); ++i)
	{
		D3D11_Texture* tex = (D3D11_Texture*)(m_colors[i].get());
		m_rtv[i] =  tex ? tex->getRTV() : NULL;
	}
}

CU_NS_END