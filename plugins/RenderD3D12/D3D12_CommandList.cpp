#include "D3D12_CommandList.h"
#include "D3D12_Device.h"
#include "D3D12_DescriptorHeap.h"
#include "D3D12_Mapping.h"

CUTE_NS_BEGIN

D3D12_CommandList::D3D12_CommandList()
	: m_cmdList(NULL)
	, m_allocator(NULL)
	, m_framebuffer(NULL)
{
}

D3D12_CommandList::~D3D12_CommandList()
{
	term();
}

bool D3D12_CommandList::init(D3D12_Device* device, COMMANDLIST_TYPE listType)
{
	HRESULT hr;
	ID3D12Device* native;

	m_device = device;
	native = device->getDevice();

	D3D12_COMMAND_LIST_TYPE type = D3D12_Mapping::getCommandListType(listType);
	hr = native->CreateCommandAllocator(type, IID_PPV_ARGS(&m_allocator));
	if (FAILED(hr))
		return false;

	hr = native->CreateCommandList(0, type, m_allocator, NULL, IID_PPV_ARGS(&m_cmdList));
	if (FAILED(hr))
		return false;

	m_cmdList->Close();
	return true;
}

void D3D12_CommandList::term()
{
	D3D12_RELEASE(m_cmdList);
	D3D12_RELEASE(m_allocator);
}

void D3D12_CommandList::begin()
{
	m_allocator->Reset();
	m_cmdList->Reset(m_allocator, NULL);
	m_framebuffer = NULL;

	D3D12_DescriptorHeap* heapBuf = m_device->getDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_DescriptorHeap* heapSmp = m_device->getDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	size_t count = 0;
	ID3D12DescriptorHeap* heaps[2] = {};
	if (heapBuf->getSize() > 0)
		heaps[count++] = heapBuf->getHeap();

	if (heapSmp->getSize() > 0)
		heaps[count++] = heapSmp->getHeap();

	if (count == 0)
		return;

	m_cmdList->SetDescriptorHeaps(count, heaps);
}

void D3D12_CommandList::end()
{
	m_cmdList->Close();
	m_framebuffer = NULL;
}

void D3D12_CommandList::setBlendConstant(const float values[4])
{
	m_cmdList->OMSetBlendFactor(values);
}

void D3D12_CommandList::setStencilReference(uint32 stencilRef)
{
	m_cmdList->OMSetStencilRef(stencilRef);
}

void D3D12_CommandList::setViewports(uint32 count, Viewport* viewports)
{
	if (count == 0)
		return;

	D3D12_VIEWPORT* pNativeViewports = reinterpret_cast<D3D12_VIEWPORT*>(viewports);
	m_cmdList->RSSetViewports(count, pNativeViewports);
}

void D3D12_CommandList::setScissors(uint32 count, Rect* scissors)
{
	if (count == 0)
		return;

	D3D12_RECT rects[D3D12_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	for (uint32 i = 0; i < count; ++i)
	{
		rects[i].left	= scissors[i].x;
		rects[i].top	= scissors[i].y;
		rects[i].right	= scissors[i].x + scissors[i].width;
		rects[i].bottom = scissors[i].y + scissors[i].height;
	}

	m_cmdList->RSSetScissorRects(count, rects);
}

void D3D12_CommandList::setPipeline(IPipeline* pipeline)
{

}

void D3D12_CommandList::setDescriptorSet(IDescriptorSet* descriptors)
{
}

void D3D12_CommandList::setVertexBuffers(uint32 startSlot, uint32 count, IBuffer* buffers, uint64 offsets)
{
}

void D3D12_CommandList::setIndexBuffer(IBuffer* buffer, uint64 offset)
{
}

void D3D12_CommandList::setBarrier(IResource* resource, RESOURCE_STATE state)
{
}

void D3D12_CommandList::drawInstanced(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance)
{
}

void D3D12_CommandList::drawIndexedInstanced(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int vertexOffset, uint32 firstInstance)
{
}

void D3D12_CommandList::dispatch(uint32 x, uint32 y, uint32 z)
{
}

void D3D12_CommandList::executeIndirect(ICommandSet* cmdSet, uint32 maxCount, IBuffer* argBuffer, uint64 argOffset, IBuffer* counterBuffer, uint64 counterOffset)
{
}

void D3D12_CommandList::executeBundle(ICommandList* cmdList)
{
}

void D3D12_CommandList::updateConstantBuffer(IBuffer* buffer, size_t offset, size_t size, const void* data)
{
}

void D3D12_CommandList::beginQuery(IQuery* query)
{
}

void D3D12_CommandList::endQuery(IQuery* query)
{
}

void D3D12_CommandList::resolveQuery()
{
}

void D3D12_CommandList::copyTexture(ITexture* dst, ITexture* src)
{
}

void D3D12_CommandList::copyBuffer(IBuffer* dst, IBuffer* src)
{
}

void D3D12_CommandList::copyTextureRegion(ITexture* dstResource, uint32 dstSubresource, const Offset3D& dstOffset, ITexture* srcResource, uint32 srcSubresource, const Offset3D& srcOffset, const Extent3D& srcExtent)
{
}

void D3D12_CommandList::copyBufferRegion(IBuffer* dstBuffer, uint64 dstOffset, IBuffer* srcBuffer, uint64 srcOffset, uint64 byteCount)
{
}

void D3D12_CommandList::copyBufferToTexture(ITexture* dstTexture, uint32 dstSubresource, const Offset3D& dstOffset, IBuffer* srcBuffer, uint64 srcOffset)
{

}

void D3D12_CommandList::copyTextureToBuffer(IBuffer* dstBuffer, uint64 dstOffset, ITexture* srcTexture, uint32 srcSubresource, const Offset3D& srcOffset, const Extent3D& srcExtent)
{
}

void D3D12_CommandList::resolveSubresource(ITexture* dstResource, uint32 dstSubresource, ITexture* srcResource, uint32 srcSubresource)
{

}

void D3D12_CommandList::pushMarker(const char* tag)
{

}

void D3D12_CommandList::popMarker()
{
}

//D3D12_CommandList::D3D12_CommandList(ID3D12Device* device, ID3D12CommandAllocator* alloc)
//{
//	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc, 0, IID_PPV_ARGS(&m_handle));
//	D3D12_CHECK(hr, "D3D12 CreateCommandList Fail!");
//}
//
//D3D12_CommandList::~D3D12_CommandList()
//{
//	D3D12_RELEASE(m_handle);
//}
//
//void D3D12_CommandList::setRenderTarget(RenderTarget* target)
//{
//
//}
//
//void D3D12_CommandList::setViewport(int x, int y, size_t w, size_t h)
//{
//	D3D12_VIEWPORT viewport = { x, y, w, h, 0.0f, 1.0f };
//	m_handle->RSSetViewports(1, &viewport);
//}
//
//void D3D12_CommandList::setScissor(int x, int y, size_t w, size_t h)
//{
//	D3D12_RECT rect = { x, y, (int)x + w, (int)y + h };
//	m_handle->RSSetScissorRects(1, &rect);
//}
//
//void D3D12_CommandList::setBlendFactor(const float factors[4])
//{
//	m_handle->OMSetBlendFactor(factors);
//}
//
//void D3D12_CommandList::setStencilRef(StencilFaceFlags mask, size_t stencilRef)
//{
//	m_handle->OMSetStencilRef(stencilRef);
//}
//
//void D3D12_CommandList::setTopology(Topology topology)
//{
//	D3D12_PRIMITIVE_TOPOLOGY primite = (D3D12_PRIMITIVE_TOPOLOGY)topology;
//	m_handle->IASetPrimitiveTopology(primite);
//}
//
//void D3D12_CommandList::setDescriptorSet(DescriptorSet* descriptors)
//{
//
//}
//
//void D3D12_CommandList::setPipeline(Pipeline* pipeline)
//{
//	m_pipeline = (D3D12_Pipeline*)pipeline;
//}
//
//void D3D12_CommandList::setVertexArray(VertexArray* vertexs)
//{
//	m_layout = (D3D12_DescriptorSetLayout*)vertexs->getLayout();
//
//	D3D12_VERTEX_BUFFER_VIEW views[CU_MAX_VERTEX_BUFFERS];
//
//	size_t counts = vertexs->getBufferCount();
//	size_t start = vertexs->getStartSlot();
//	for (size_t i = 0; i < counts; ++i)
//	{
//		D3D12_Buffer* buffer = (D3D12_Buffer*)vertexs->getBuffer(i);
//		views[i].BufferLocation = buffer->getGPUVirtualAddress();
//		views[i].SizeInBytes = buffer->bytes();
//		views[i].StrideInBytes = buffer->stride();
//	}
//	m_handle->IASetVertexBuffers(start, counts, views);
//}
//
//void D3D12_CommandList::setIndexBuffer(IndexBuffer* buffer, size_t offset)
//{
//	D3D12_Buffer* dx_buffer = (D3D12_Buffer*)buffer;
//	D3D12_INDEX_BUFFER_VIEW view;
//	view.BufferLocation = dx_buffer->getGPUVirtualAddress() + offset;
//	view.SizeInBytes = dx_buffer->bytes() - offset;
//	view.Format = buffer->isIndex16() ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
//	m_handle->IASetIndexBuffer(&view);
//}
//
//void D3D12_CommandList::clear(ClearMask masks, const Color& color, float depth, uint32_t stencil, uint8_t targetMask)
//{
//
//}
//
//void D3D12_CommandList::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
//{
//	prepare();
//	m_handle->DrawInstanced(vertexCount, instanceCount, vertexOffset, instanceOffset);
//}
//
//void D3D12_CommandList::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
//{
//	prepare();
//	m_handle->DrawIndexedInstanced(indexCount, instanceCount, indexOffset, vertexOffset, instanceCount);
//}
//
//void D3D12_CommandList::dispatch(size_t x, size_t y, size_t z)
//{
//	m_handle->Dispatch(x, y, z);
//}
//
//void D3D12_CommandList::prepare()
//{
//	if (m_pipeline)
//		m_pipeline->bind(this);
//}

CUTE_NS_END
