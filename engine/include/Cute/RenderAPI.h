#pragma once
#include "Cute/RenderDesc.h"
#include "Cute/Singleton.h"

CUTE_NS_BEGIN

// a3d
//https://github.com/ProjectAsura/asura-SDK/tree/master/a3d

//////////////////////////////////////////////////////////////////////////
// graphics interface:GI_XXX, IGFX_XXXX or GFX_XXX
//////////////////////////////////////////////////////////////////////////
// GBuffer GTexture wrapper for IBuffer and ITexture

//class CUTE_CORE_API IResource : public Object
//{
//	DECLARE_RTTI(IResource, Object, OID_ANY)
//public:
//	virtual ~IResource(){}
//
//	virtual bool map(void_ptr& reslut) = 0;
//	virtual void unmap() = 0;
//
//	virtual RESOURCE_STATE getState() const = 0;
//};

class CUTE_CORE_API IBuffer : public Object
{
	DECLARE_RTTI(IBuffer, Object, OID_ANY)
public:
    IBuffer(const BufferDesc& desc);
	virtual ~IBuffer(){}
    
    virtual bool map(void_ptr& result, RESOURCE_MAP mapMode) = 0;
    virtual void unmap() = 0;
    
    RESOURCE_USAGE      getUsage() const { return m_usage; }
    uint                getSize() const { return m_size; }
    uint                getStride() const { return m_stride; }

protected:
    RESOURCE_USAGE  m_usage;
    uint            m_size;
    uint            m_stride;
};

class CUTE_CORE_API ITexture : public Object
{
	DECLARE_RTTI(ITexture, Object, OID_ANY)
public:
    ITexture(const TextureDesc& desc);
	virtual ~ITexture(){}
    
    virtual bool map(void_ptr& result, RESOURCE_MAP mapMode) = 0;
    virtual void unmap() = 0;

	virtual bool getSubresourceLayout(SubresourceLayout& layout, uint32 subresource) const = 0;
    
    RESOURCE_DIMENSION  getDimension() const { return m_dimension; }
    RESOURCE_FORMAT     getFormat() const { return m_format; }
    uint32              getWidth() const { return m_width; }
    uint32              getHeight() const { return m_height; }
    uint32              getDepth() const { return m_depth; }
    uint32              getArraySize() const { return m_depth; }

protected:
    RESOURCE_DIMENSION  m_dimension;
    RESOURCE_FORMAT     m_format;
    uint32              m_width;
    uint32              m_height;
    uint32              m_depth;
    uint32              m_mipmaps;
};

class CUTE_CORE_API ISampler : public Object
{
    DECLARE_RTTI(ISampler, Object, OID_ANY)
public:
    virtual ~ISampler(){}
};

class CUTE_CORE_API IFrameBuffer : public Object
{
	DECLARE_RTTI(IFrameBuffer, Object, OID_ANY)
public:
	virtual ~IFrameBuffer(){}
};

class CUTE_CORE_API ISwapChain : public Object
{
	DECLARE_RTTI(ISwapChain, Object, OID_ANY)
public:
	virtual ~ISwapChain(){}

	virtual void present() = 0;
	virtual bool resize(uint32 width, uint32 height) = 0;
	virtual bool getBuffer(uint32 index, ITexture* texture) = 0;
	virtual bool setMetaData(META_DATA_TYPE type, void* data) = 0;
	virtual void setFullScreen(bool enable) = 0;
	virtual bool isFullScreen() const = 0;
	virtual bool checkColorSpaceSupport(COLOR_SPACE_TYPE type, uint32* flags) = 0;
};

class CUTE_CORE_API IVertexLayout : public Object
{
    DECLARE_RTTI(IVertexLayout, Object, OID_ANY)
public:
    virtual ~IVertexLayout(){}
};

class CUTE_CORE_API IDescriptorSet : public Object
{
	DECLARE_RTTI(IDescriptorSet, Object, OID_ANY)
public:
	virtual ~IDescriptorSet(){}

	virtual void setBuffer(size_t index, IBuffer* buffer) = 0;
	virtual void setTexture(size_t index, ITexture* texture) = 0;
	virtual void setSampler(size_t index, ISampler* sampler) = 0;

	virtual void update() {}
};

class CUTE_CORE_API IDescriptorSetLayout : public Object
{
	DECLARE_RTTI(IDescriptorSetLayout, Object, OID_ANY)
public:
	virtual ~IDescriptorSetLayout(){}
};

// gpu program
class CUTE_CORE_API IShader : public Object
{
	DECLARE_RTTI(IShader, Object, OID_ANY)
public:
	virtual ~IShader(){}
    virtual bool compile(const ShaderDesc& desc) = 0;
};

//class CUTE_CORE_API IShaderModule : public Object
//{
//	DECLARE_RTTI(IShaderModule, Object, OID_ANY)
//public:
//	virtual ~IShaderModule(){}
//
//	virtual bool compile(const ShaderDesc& desc) = 0;
//};

class CUTE_CORE_API IPipeline : public Object
{
	DECLARE_RTTI(IPipeline, Object, OID_ANY)
public:
	virtual ~IPipeline(){}
};

class CUTE_CORE_API IFence : public Object
{
	DECLARE_RTTI(IFence, Object, OID_ANY)
public:
	virtual ~IFence(){}

	virtual bool isSignaled() const = 0;
	virtual bool wait(uint32 msec) = 0;
};

class CUTE_CORE_API IQuery : public Object
{
	DECLARE_RTTI(IQuery, Object, OID_ANY)
public:
	virtual ~IQuery(){}
};

class CUTE_CORE_API ICommandSet : public Object
{
	DECLARE_RTTI(ICommandSet, Object, OID_ANY)
public:
	virtual ~ICommandSet(){}
};

// CommandBuffer
class CUTE_CORE_API ICommandList : public Object
{
	DECLARE_RTTI(ICommandList, Object, OID_ANY)
public:
	virtual ~ICommandList(){}

	virtual void begin() = 0;
	virtual void end() = 0;

	virtual void setBlendConstant(const float values[4]) = 0;
	virtual void setStencilReference(uint32 stencilRef) = 0;
	virtual void setViewports(uint32 count, Viewport* viewports) = 0;
	virtual void setScissors(uint32 count, Rect* scissors) = 0;
	virtual void setPipeline(IPipeline* pipeline) = 0;
	virtual void setDescriptorSet(IDescriptorSet* descriptors) = 0;
	virtual void setVertexBuffers(uint32 startSlot, uint32 count, IBuffer* buffers, uint64 offsets) = 0;
	virtual void setIndexBuffer(IBuffer* buffer, uint64 offset) = 0;

//	virtual void setBarrier(IResource* resource, RESOURCE_STATE state) = 0;

	virtual void drawInstanced(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) = 0;
	virtual void drawIndexedInstanced(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int vertexOffset, uint32 firstInstance) = 0;
	virtual void dispatch(uint32 x, uint32 y, uint32 z) = 0;
	virtual void executeIndirect(ICommandSet* cmdSet, uint32 maxCount, IBuffer* argBuffer, uint64 argOffset, IBuffer* counterBuffer, uint64 counterOffset) = 0;
	virtual void executeBundle(ICommandList* cmdList) = 0;

	virtual void updateConstantBuffer(IBuffer* buffer, size_t offset, size_t size, const void* data) = 0;

	virtual void beginQuery(IQuery* query) = 0;
	virtual void endQuery(IQuery* query) = 0;
	virtual void resolveQuery() = 0;

	virtual void copyTexture(ITexture* dst, ITexture* src) = 0;
	virtual void copyBuffer(IBuffer* dst, IBuffer* src) = 0;
	virtual void copyTextureRegion(
		ITexture*		dstResource,
		uint32			dstSubresource,
		const Offset3D& dstOffset,
		ITexture*		srcResource,
		uint32			srcSubresource,
		const Offset3D& srcOffset,
		const Extent3D& srcExtent) = 0;

	virtual void copyBufferRegion(
		IBuffer*		dstBuffer,
		uint64			dstOffset,
		IBuffer*		srcBuffer,
		uint64			srcOffset,
		uint64			byteCount) = 0;

	virtual void copyBufferToTexture(
		ITexture*		dstTexture,
		uint32			dstSubresource,
		const Offset3D&	dstOffset,
		IBuffer*		srcBuffer,
		uint64			srcOffset) = 0;

	virtual void copyTextureToBuffer(
		IBuffer*		dstBuffer,
		uint64			dstOffset,
		ITexture*		srcTexture,
		uint32			srcSubresource,
		const Offset3D& srcOffset,
		const Extent3D& srcExtent) = 0;

	virtual void resolveSubresource(
		ITexture*		dstResource,
		uint32			dstSubresource,
		ITexture*		srcResource,
		uint32			srcSubresource) = 0;

	virtual void pushMarker(const char* tag) = 0;
	virtual void popMarker() = 0;
};

class CUTE_CORE_API ICommandQueue : public Object
{
	DECLARE_RTTI(ICommandQueue, Object, OID_ANY)
public:
	virtual ~ICommandQueue(){}

	virtual bool submit(ICommandList* cmdList) = 0;
	virtual void execute(IFence* fence) = 0;
	virtual void waitIdle() = 0;
};

class CUTE_CORE_API IDevice : public Object
{
	DECLARE_RTTI(IDevice, Object, OID_ANY)
public:
	virtual ~IDevice(){}

	virtual bool getGraphicsQueue(ICommandQueue** ppQueue) = 0;
	virtual bool getComputeQueue(ICommandQueue** ppQueue) = 0;
	virtual bool getCopyQueue(ICommandQueue** ppQueue) = 0;

	virtual bool newBuffer(const BufferDesc* desc, IBuffer** buffer) = 0;
	virtual bool newTexture(const TextureDesc* desc, ITexture** texture) = 0;
	virtual bool newCommandBuffer(COMMANDLIST_TYPE type, ICommandList** cmdBuffer) = 0;
	virtual bool newFrameBuffer() = 0;
	virtual bool newSwapChain(ISwapChain** swapchain) = 0;
};

class CUTE_CORE_API IDeviceFactory : public Object
{
public:
	virtual ~IDeviceFactory(){}

    virtual const String&   name() const = 0;
    virtual IDevice*        create() const = 0;
//	virtual bool getDevice(const DeviceDesc& desc, IDevice** device) = 0;
};

// class RenderModule
class CUTE_CORE_API RenderModule : public Singleton<RenderModule>
{
public:
    RenderModule();
    ~RenderModule();
    
    void addFactory(IDeviceFactory* factory);
    void delFactory(IDeviceFactory* factory);
    
    void setDevice(const String& name);
    
    IDeviceFactory* getFactory(const String& name);
    IDevice*        getDevice();
    
private:
    typedef Vector<IDeviceFactory*> FactoryArray;
    FactoryArray m_factories;
    IDevice*     m_device;
};

CUTE_NS_END
