#include "D3D11_CommandBuffer.h"
#include "D3D11_Buffer.h"
#include "D3D11_Pipeline.h"
#include "D3D11_VertexLayout.h"
#include "D3D11_DescriptorSet.h"
#include "D3D11_FrameBuffer.h"
#include "D3D11_SwapChain.h"

CUTE_NS_BEGIN

D3D11_CommandBuffer::D3D11_CommandBuffer(ID3D11DeviceN* device, ID3D11ContextN* context)
	: m_context(context)
	, m_target(NULL)
	, m_pipeline(NULL)
	, m_layout(NULL)
	, m_descriptors(NULL)
{
	HRESULT hr;
	if (m_context == NULL)
	{
		hr = device->CreateDeferredContext(0, &m_context);
		D3D11_CHECK(hr, "CreateDeferredContext failed.");
	}
	memset(m_factors, 0, sizeof(m_factors));
	m_stencilRef = 0;
}

D3D11_CommandBuffer::~D3D11_CommandBuffer()
{

}

void D3D11_CommandBuffer::setRenderTarget(RenderTarget* target)
{
	m_target = target;
	target->bind(m_context);
}

void D3D11_CommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	D3D11_VIEWPORT viewport = { x, y, w, h, 0.0f, 1.0f };
	m_context->RSSetViewports(1, &viewport);
}

void D3D11_CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{
	D3D11_RECT rect = { x, y, (int)x + w, (int)y + h};
	m_context->RSSetScissorRects(1, &rect);
}

void D3D11_CommandBuffer::setBlendFactor(const float factors[4])
{
	memcpy(m_factors, factors, sizeof(m_factors));
}

void D3D11_CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	m_stencilMask = mask;
	m_stencilRef = reference;
}

void D3D11_CommandBuffer::setTopology(Topology topology)
{
	D3D11_PRIMITIVE_TOPOLOGY primite = (D3D11_PRIMITIVE_TOPOLOGY)topology;
	m_context->IASetPrimitiveTopology(primite);
}

void D3D11_CommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{
	m_descriptors = descriptors->cast<D3D11_DescriptorSet>();
}

void D3D11_CommandBuffer::setPipeline(Pipeline* pipeline)
{
	m_pipeline = pipeline->cast<D3D11_Pipeline>();
}

void D3D11_CommandBuffer::setVertexArray(VertexArray* vertexs)
{
	m_layout = (D3D11_VertexLayout*)vertexs->getLayout();

	ID3D11Buffer* dxBuffers[CU_MAX_VERTEX_BUFFERS];
	UINT32 dxStrides[CU_MAX_VERTEX_BUFFERS];
	UINT32 dxOffsets[CU_MAX_VERTEX_BUFFERS];

	size_t counts = vertexs->getBufferCount();
	size_t start = vertexs->getStartSlot();
	D3D11_Buffer* buffer;
	for (size_t i = 0; i < counts; ++i)
	{
		buffer = (D3D11_Buffer*)vertexs->getBuffer(i);

		dxBuffers[i] = buffer->native();
		dxStrides[i] = buffer->stride();
		dxOffsets[i] = vertexs->getOffset(i);
	}

	m_context->IASetVertexBuffers(start, counts, dxBuffers, dxStrides, dxOffsets);
}

void D3D11_CommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{
	D3D11_Buffer* dx_buffer = (D3D11_Buffer*)buffer;
	DXGI_FORMAT format = buffer->isIndex16() ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_context->IASetIndexBuffer(dx_buffer->native(), format, offset);
}

void D3D11_CommandBuffer::clear(ClearMask masks, const Color& color, float depth, uint32_t stencil, uint8_t targetMask)
{
	if (!m_target)
		return;

	if (masks & CLEAR_COLOR)
	{
		if (m_target->isKindOf<D3D11_SwapChain>())
		{
			D3D11_SwapChain* chain = (D3D11_SwapChain*)m_target;
			chain->clearRTV(m_context, color);
		}
		else
		{
			D3D11_FrameBuffer* fbo = (D3D11_FrameBuffer*)m_target;
			fbo->clearRTV(m_context, color, targetMask);
		}
	}

	if (masks & CLEAR_DEPTH_STENCIL)
	{
		UINT32 clearFlag = 0;
		if (masks & CLEAR_DEPTH)
			clearFlag |= D3D11_CLEAR_DEPTH;
		if (masks & CLEAR_STENCIL)
			clearFlag |= D3D11_CLEAR_STENCIL;

		if (m_target->isKindOf<D3D11_SwapChain>())
		{
			D3D11_SwapChain* chain = (D3D11_SwapChain*)m_target;
			chain->clearDSV(m_context, clearFlag, depth, stencil);
		}
		else
		{
			D3D11_FrameBuffer* fbo = (D3D11_FrameBuffer*)m_target;
			fbo->clearDSV(m_context, clearFlag, depth, stencil);
		}
	}
}

void D3D11_CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	prepare();
	if (instanceCount == 0)
	{
		m_context->Draw(vertexCount, vertexOffset);
	}
	else
	{
		m_context->DrawInstanced(vertexCount, instanceCount, vertexOffset, instanceOffset);
	}
}

void D3D11_CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{
	prepare();
	if (instanceCount == 0)
	{
		m_context->DrawIndexed(indexCount, indexOffset, vertexOffset);
	}
	else
	{
		m_context->DrawIndexedInstanced(indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
	}
}

void D3D11_CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{
	m_context->Dispatch(x, y, z);
}

void D3D11_CommandBuffer::prepare()
{
	if (m_pipeline)
		m_pipeline->bind(this);
	if (m_descriptors)
		m_descriptors->bind(m_context);
}

CUTE_NS_END
