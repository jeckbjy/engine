#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class VK_FrameBuffer;
class CUTE_VK_API VK_CommandList : public ICommandList
{
public:
	VK_CommandList();
	~VK_CommandList();

	bool init(VK_Device* device, COMMANDLIST_TYPE type);
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

	//void setRenderTarget(RenderTarget* target);
	//void setViewport(int x, int y, size_t w, size_t h);
	//void setScissor(int x, int y, size_t w, size_t h);
	//void setBlendFactor(const float factors[4]);
	//void setStencilRef(StencilFaceFlags mask, size_t reference);

	//void setTopology(Topology topology);
	//void setDescriptorSet(DescriptorSet* descriptors);
	//void setPipeline(Pipeline* pipeline);
	//void setVertexArray(VertexArray* vertexs);
	//void setIndexBuffer(IndexBuffer* buffer, size_t offset);

	//void clear(ClearMask masks /* = CLEAR_ALL */, const Color& color /* = Color::BLACK */, float depth /* = 1.0f */, uint32_t stencil /* = 0 */, uint8_t targetMask /* = 0xFF */);
	//void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset);
	//void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset);
	//void dispatch(size_t x, size_t y, size_t z);

private:
	VK_Device*		m_device;
	VK_FrameBuffer*	m_frame;
	VkCommandPool	m_pool;
	VkCommandBuffer	m_buffer;
};

CUTE_NS_END
