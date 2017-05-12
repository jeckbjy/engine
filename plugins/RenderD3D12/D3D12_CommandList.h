#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class D3D12_Device;
class D3D12_FrameBuffer;
class CUTE_D3D12_API D3D12_CommandList : public ICommandList
{
public:
	D3D12_CommandList();
	~D3D12_CommandList();

	bool init(D3D12_Device* device, COMMANDLIST_TYPE type);
	void term();

	void begin() OVERRIDE;
	void end() OVERRIDE;

	void setBlendConstant(const float values[4]) OVERRIDE;
	void setStencilReference(uint32 stencilRef) OVERRIDE;
	void setViewports(uint32 count, Viewport* viewports) OVERRIDE;
	void setScissors(uint32 count, Rect* scissors) OVERRIDE;
	void setPipeline(IPipeline* pipeline) OVERRIDE;
	void setDescriptorSet(IDescriptorSet* descriptors) OVERRIDE;
	void setVertexBuffers(uint32 startSlot, uint32 count, IBuffer* buffers, uint64 offsets) OVERRIDE;
	void setIndexBuffer(IBuffer* buffer, uint64 offset) OVERRIDE;

	void setBarrier(IResource* resource, RESOURCE_STATE state) OVERRIDE;

	void drawInstanced(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) OVERRIDE;
	void drawIndexedInstanced(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int vertexOffset, uint32 firstInstance) OVERRIDE;
	void dispatch(uint32 x, uint32 y, uint32 z) OVERRIDE;
	void executeIndirect(ICommandSet* cmdSet, uint32 maxCount, IBuffer* argBuffer, uint64 argOffset, IBuffer* counterBuffer, uint64 counterOffset) OVERRIDE;
	void executeBundle(ICommandList* cmdList) OVERRIDE;

	void updateConstantBuffer(IBuffer* buffer, size_t offset, size_t size, const void* data) OVERRIDE;

	void beginQuery(IQuery* query) OVERRIDE;
	void endQuery(IQuery* query) OVERRIDE;
	void resolveQuery() OVERRIDE;

	void copyTexture(ITexture* dst, ITexture* src) OVERRIDE;
	void copyBuffer(IBuffer* dst, IBuffer* src) OVERRIDE;
	void copyTextureRegion(
		ITexture*		dstResource,
		uint32			dstSubresource,
		const Offset3D& dstOffset,
		ITexture*		srcResource,
		uint32			srcSubresource,
		const Offset3D& srcOffset,
		const Extent3D& srcExtent) OVERRIDE;

	void copyBufferRegion(
		IBuffer*		dstBuffer,
		uint64			dstOffset,
		IBuffer*		srcBuffer,
		uint64			srcOffset,
		uint64			byteCount) OVERRIDE;

	void copyBufferToTexture(
		ITexture*		dstTexture,
		uint32			dstSubresource,
		const Offset3D&	dstOffset,
		IBuffer*		srcBuffer,
		uint64			srcOffset) OVERRIDE;

	void copyTextureToBuffer(
		IBuffer*		dstBuffer,
		uint64			dstOffset,
		ITexture*		srcTexture,
		uint32			srcSubresource,
		const Offset3D& srcOffset,
		const Extent3D& srcExtent) OVERRIDE;

	void resolveSubresource(
		ITexture*		dstResource,
		uint32			dstSubresource,
		ITexture*		srcResource,
		uint32			srcSubresource) OVERRIDE;

	void pushMarker(const char* tag) OVERRIDE;
	void popMarker() OVERRIDE;
private:
	D3D12_Device*				m_device;
	ID3D12GraphicsCommandList*	m_cmdList;
	ID3D12CommandAllocator*		m_allocator;
	D3D12_FrameBuffer*			m_framebuffer;
};

//class D3D12_Pipeline;
//class D3D12_DescriptorSetLayout;
//class CUTE_D3D12_API D3D12_CommandList : public CommandBuffer
//{
//public:
//	D3D12_CommandList(ID3D12Device* device, ID3D12CommandAllocator* alloc);
//	~D3D12_CommandList();
//
//	void setRenderTarget(RenderTarget* target);
//	void setViewport(int x, int y, size_t w, size_t h);
//	void setScissor(int x, int y, size_t w, size_t h);
//	void setBlendFactor(const float factors[4]);
//	void setStencilRef(StencilFaceFlags mask, size_t stencilRef);
//	void setTopology(Topology topology);
//	void setDescriptorSet(DescriptorSet* descriptors);
//	void setPipeline(Pipeline* pipeline);
//	void setVertexArray(VertexArray* vertexs);
//	void setIndexBuffer(IndexBuffer* buffer, size_t offset);
//
//	void clear(ClearMask masks /* = CLEAR_ALL */, const Color& color /* = Color::BLACK */, float depth /* = 1.0f */, uint32_t stencil /* = 0 */, uint8_t targetMask /* = 0xFF */);
//	void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
//	void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
//	void dispatch(size_t x, size_t y, size_t z);
//
//	ID3D12GraphicsCommandList* native() { return m_handle; }
//
//	D3D12_DescriptorSetLayout* getLayout() { return m_layout; }
//
//private:
//	void prepare();
//
//private:
//	ID3D12GraphicsCommandList*	m_handle;
//	D3D12_DescriptorSetLayout*			m_layout;
//	D3D12_Pipeline*				m_pipeline;
//};

CUTE_NS_END
