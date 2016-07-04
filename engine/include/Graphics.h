#pragma once
#include "Object.h"
#include "SmartPtr.h"
#include "GraphicsDesc.h"

CU_NS_BEGIN

class CU_API GpuResource : public Object
{
	DECLARE_RTTI(GpuResource, Object, OBJ_ID_GPU_RESOURCE);
public:
	virtual ~GpuResource() {}
};

class CU_API GpuBuffer : public GpuResource
{
	DECLARE_RTTI(GpuBuffer, GpuResource, OBJ_ID_GPU_BUFFER);
public:
	GpuBuffer(const BufferDesc& desc);
	virtual ~GpuBuffer();

	virtual void* map(size_t offset, size_t len, MAP_FLAG access) = 0;
	virtual void  unmap() = 0;

	virtual void write(const void* data, size_t len, size_t offset = 0);
	virtual void read(void* data, size_t len, size_t offset = 0);

	size_t bytes() const { return m_bytes; }
	size_t count() const { return m_count; }
	size_t stride() const { return m_stride; }

	bool hasFlag(uint32 mask) const { return (m_flags & mask) == mask; }
	bool isDynamic() const { return hasFlag(RES_DYNAMIC); }
	bool isIndex16() const { return m_stride == sizeof(int16_t); }

protected:
	uint32 m_flags;
	size_t m_bytes;
	size_t m_count;
	size_t m_stride;
};

typedef GpuBuffer VertexBuffer;
typedef GpuBuffer IndexBuffer;
typedef GpuBuffer UniformBuffer;

class CU_API Texture : public GpuResource
{
	DECLARE_RTTI(Texture, GpuResource, OBJ_ID_TEXTURE);
public:
	Texture(const TextureDesc& desc){}
	virtual ~Texture(){}

	virtual void* map(PixelData& data, MAP_FLAG flag, uint level = 0, uint face = 0) = 0;
	virtual void  unmap() = 0;

protected:
	PixelFormat m_format;
	TexType		m_type;
	RES_FLAG	m_usage;
	size_t		m_width;
	size_t		m_height;
	size_t		m_depth;
	size_t		m_mipmaps;
};

class CU_API VertexLayout : public Object
{
	DECLARE_RTTI(VertexLayout, Object, OBJ_ID_INPUT_LAYOUT)
public:
	typedef std::vector<VertexElement> ElementVec;

	static uint32_t hash(const VertexElement* elements, size_t count);

	VertexLayout(const VertexElement* elements, size_t count);
	virtual ~VertexLayout(){}

	bool equal(const VertexElement* elements, size_t count) const;
	const ElementVec& getElements() const { return m_elements; }

protected:
	ElementVec	m_elements;
	uint32_t	m_hash;
	bool		m_instanced;
};

// 含有顶点结构和多个VertexBuffer, VertexArray
class CU_API VertexArray : public Object
{
public:
	typedef std::vector<GpuBufferPtr> BufferArray;
	typedef std::vector<uint32_t>	  OffsetArray;

	VertexArray(VertexLayout* layout);
	virtual ~VertexArray(){}

	void setLayout(VertexLayout* layout);
	void setBuffer(GpuBuffer* buffer, size_t slot = 0, size_t offset = 0);
	void setStartSlot(size_t start) { m_startSlot = start; }

	VertexLayout*	getLayout() { return m_layout.get(); }
	size_t			getBufferCount() { return m_buffers.size(); }
	GpuBuffer*		getBuffer(size_t slot) { return m_buffers[slot]; }
	size_t			getOffset(size_t slot) { return m_offsets[slot]; }
	BufferArray&	getBufferArray() { return m_buffers; }
	OffsetArray&	getOffsetArray() { return m_offsets; }
	size_t			getStartSlot() const { return m_startSlot; }

protected:
	bool			m_dirty;
	VertexLayoutPtr m_layout;
	BufferArray		m_buffers;
	OffsetArray		m_offsets;
	size_t			m_startSlot;
};

class CU_API RenderTarget : public Object
{
	DECLARE_RTTI(RenderTarget, Object, OBJ_ID_RENDER_TARGET);
public:
	virtual ~RenderTarget(){}
	// 内部调用使用
	virtual void bind(void* param) = 0;
};

class CU_API FrameBuffer : public RenderTarget
{
	DECLARE_RTTI(FrameBuffer, RenderTarget, OBJ_ID_FRAMEBUFFER);
public:
	FrameBuffer() :m_dirty(false){}
	virtual ~FrameBuffer(){}

	virtual void attach(size_t att, Texture* attachment);
	virtual void detach(size_t att);

protected:
	typedef SharedPtr<Texture>	TexturePtr;
	typedef Vector<TexturePtr>	TextureVec;
	TextureVec	m_attachments;
	bool		m_dirty;
};

class CU_API SwapChain : public RenderTarget
{
	DECLARE_RTTI(SwapChain, RenderTarget, OBJ_ID_SWAPCHAIN);
public:
	virtual ~SwapChain(){}
	virtual void present() = 0;
};

class CU_API ShaderStage : public Object
{
	DECLARE_RTTI(ShaderStage, Object, OBJ_ID_SHADER_STAGE);
public:
	ShaderStage(){}
	virtual ~ShaderStage(){}

	virtual bool compile(const ShaderDesc& desc) = 0;
	ShaderType getShaderType() const { return m_type; }

protected:
	ShaderType m_type;
};

class CU_API ShaderProgram : public Object
{
	DECLARE_RTTI(ShaderProgram, Object, OBJ_ID_SHADER_PROTRAM);
public:
	virtual ~ShaderProgram(){}
	virtual void attach(ShaderStage* shader) = 0;
	virtual void link() = 0;
};

// used for material
class CU_API DescriptorSet : public Object
{
	DECLARE_RTTI(DescriptorSet, Object, OBJ_ID_DESCRIPTOR_SET);
public:
	virtual ~DescriptorSet(){}
	//virtual void setValue(const String& name, GpuBuffer* value, size_t index) = 0;
	virtual void setValue(const String& name, Texture* texture, size_t index = 0) = 0;
	virtual void setValue(const String& name, const void* data, size_t size, size_t offset) = 0;

	// 单个普通数据
	template<typename T>
	void setValue(const String& name, const T& value, size_t index = 0)
	{
		this->setValue(name, (void*)&value, sizeof(T), sizeof(T) * index);
	}

	// 数组
	template<typename T>
	void setValue(const String& name, const T* value, size_t count = 1, size_t offset = 0)
	{
		this->setValue(name, (void*)value, sizeof(T) * count, sizeof(T)* offset);
	}
};

// 渲染管线,计算管线
class CU_API Pipeline : public Object
{
	DECLARE_RTTI(Pipeline, Object, OBJ_ID_PIPELINE);
protected:
	virtual ~Pipeline(){}
	//virtual PipelineType type() const = 0;
};

class CU_API RenderPass : public Object
{
	DECLARE_RTTI(RenderPass, Object, OBJ_ID_RENDER_PASS);
protected:
	virtual ~RenderPass(){}
};

// execute buffer
class CU_API CommandBuffer : public Object
{
	DECLARE_RTTI(CommandBuffer, Object, OBJ_ID_COMMAND_BUFFER);
public:
	virtual ~CommandBuffer(){}

	virtual void setRenderTarget(RenderTarget* target) = 0;
	virtual void setViewport(int x, int y, size_t w, size_t h) = 0;
	virtual void setScissor(int x, int y, size_t w, size_t h) = 0;
	virtual void setBlendFactor(const float factors[4]) = 0;
	virtual void setStencilRef(StencilFaceFlags mask, size_t reference) = 0;
	virtual void setTopology(Topology topology) = 0;
	virtual void setDescriptorSet(DescriptorSet* descriptors) = 0;
	virtual void setPipeline(Pipeline* pipeline) = 0;
	virtual void setVertexArray(VertexArray* vertexs) = 0;
	virtual void setIndexBuffer(IndexBuffer* buffer, size_t offset = 0) = 0;

	virtual void clear(ClearMask mask = CLEAR_ALL, const Color& color = Color::BLACK, float depth = 1.0f, uint32_t stencil = 0, uint8_t targetMask = 0xFF) = 0;
	virtual void draw(uint32_t vertexCount, uint32_t instanceCount = 0, uint32_t vertexOffset = 0, uint32_t instanceOffset = 0) = 0;
	virtual void drawIndexed(uint32_t indexCount, uint32_t instanceCount = 0, uint32_t indexOffset = 0, uint32_t instanceOffset = 0, uint32_t vertexOffset = 0) = 0;
	virtual void dispatch(size_t x, size_t y, size_t z) = 0;
};

class CU_API Fence : public Object
{
	DECLARE_RTTI(Fence, Object, OBJ_ID_FENCE);
public:
	virtual void wait() = 0;
	virtual void wait(uint64_t timeout) = 0;
};

// 队列
class CU_API CommandQueue : public Object
{
	DECLARE_RTTI(CommandQueue, Object, OBJ_ID_COMMAND_QUEUE);
public:
	virtual ~CommandQueue(){}
	virtual void submit(CommandBuffer* cmds, Fence* fence) = 0;
	//virtual void waitIdle() = 0;
};

// 底层渲染接口,mean Device create objs
class CU_API Device : public Object
{
	DECLARE_RTTI(Device, Object, OBJ_ID_DEVICE);
public:
	virtual ~Device(){}

	virtual GpuBuffer*		newBuffer(const BufferDesc& desc) = 0;
	virtual Texture*		newTexture(const TextureDesc& desc) = 0;
	virtual VertexLayout*	newVertexLayout(const VertexElement* elements, size_t count);
	virtual VertexArray*	newVertexArray(VertexLayout* layout);
	virtual ShaderStage*	newShader() = 0;
	virtual ShaderProgram*	newProgram() = 0;
	virtual Pipeline*		newPipeline(const PipelineDesc& desc) = 0;
	virtual DescriptorSet*	newDescriptorSet(Pipeline* pipeline) = 0;
	virtual CommandBuffer*	newCommandBuffer() = 0;
	virtual CommandQueue*	newCommandQueue() = 0;
	virtual FrameBuffer*	newFrameBuffer() = 0;
	virtual SwapChain*		newSwapChain(const SwapChainDesc& desc) = 0;

	GpuBuffer* newVertexBuffer(uint32_t stride, uint32_t counts, const void* data = NULL, RES_FLAG flags = RES_DEFAULT);
	GpuBuffer* newIndexBuffer(IndexType type, uint32_t counts, const void* data = NULL, RES_FLAG flags = RES_DEFAULT);
	GpuBuffer* newUniformBuffer(uint32_t bytes, const void* data = NULL, RES_FLAG flags = RES_DYNAMIC);
	ShaderStage* loadShader(ShaderType type, const String& path);
};

// 用于全局,枚举GPU
class CU_API Graphics : public Object
{
	DECLARE_RTTI(Graphics, Object, OBJ_ID_GRAPHICS);
public:
	virtual ~Graphics(){}

	void setDevice(Device* device){ m_device = device; }
	Device* getDevice() { return m_device.get(); }

private:
	SharedPtr<Device> m_device;
};

// 全局接口
extern CU_API Graphics gGraphics;

CU_NS_END
