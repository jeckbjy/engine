#pragma once
#include "Object.h"
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

// vertex顶点结构
class CU_API InputLayout : public Object
{
	DECLARE_RTTI(InputLayout, Object, OBJ_ID_INPUT_LAYOUT)
public:
	static uint32_t hash(const InputElement* elements, size_t count);

	InputLayout(const InputElement* elements, size_t count);
	virtual ~InputLayout(){}

	bool equal(const InputElement* elements, size_t count) const;

protected:
	typedef std::vector<InputElement> ElementVec;
	ElementVec	m_elements;
	uint32_t	m_hash;
};

class CU_API RenderTarget : public Object
{
	DECLARE_RTTI(RenderTarget, Object, OBJ_ID_RENDER_TARGET);
public:
	virtual ~RenderTarget(){}
	virtual void present() = 0;
	virtual void clear(uint32_t flags = CLEAR_ALL, const Color& color = Color::BLACK, float depth = 1.0f, int32_t stencil = 0) = 0;
};

class CU_API Program : public Object
{
	DECLARE_RTTI(Program, Object, OBJ_ID_PROTRAM);
public:
	virtual ~Program(){}

	virtual bool compile(const ProgramDesc& desc) = 0;

	uint getID() { return m_id; }

private:
	uint m_id;
};

// used for material
class CU_API DescriptorSet : public Object
{
	DECLARE_RTTI(DescriptorSet, Object, OBJ_ID_DESCRIPTOR_SET);
public:
	virtual ~DescriptorSet(){}
	virtual void bind(const String& name, GpuResource* res) = 0;
};

// 描述DescriptorSet使用
class CU_API PipelineLayout : public Object
{
	DECLARE_RTTI(PipelineLayout, Object, OBJ_ID_PIPELINE_LAYOUT);
public:
	virtual ~PipelineLayout(){}
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
protected:
	virtual ~RenderPass(){}
};

// execute buffer
class CU_API CommandBuffer : public Object
{
	DECLARE_RTTI(CommandBuffer, Object, OBJ_ID_COMMAND_BUFFER);
public:
	virtual ~CommandBuffer(){}

	virtual void reset() = 0;
	virtual void setViewport(int x, int y, size_t w, size_t h) = 0;
	virtual void setScissor(int x, int y, size_t w, size_t h) = 0;
	virtual void setBlendFactor(const float factors[4]) = 0;
	virtual void setStencilRef(StencilFaceFlags mask, size_t reference) = 0;
	virtual void setRenderTarget(RenderTarget* target) = 0;
	virtual void setDescriptorSet(DescriptorSet* descriptors) = 0;
	virtual void setPipeline(Pipeline* pipeline) = 0;
	virtual void setInputLayout(InputLayout* layout) = 0;
	virtual void setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets) = 0;
	virtual void setIndexBuffer(IndexBuffer* buffer, size_t offset) = 0;
	virtual void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset) = 0;
	virtual void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset) = 0;
	virtual void dispatch(size_t x, size_t y, size_t z) = 0;
};

class CU_API Fence
{
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
	virtual void waitIdle() = 0;
};

// 底层渲染接口,mean Device create objs
class CU_API Device : public Object
{
	DECLARE_RTTI(Device, Object, OBJ_ID_DEVICE);
public:
	virtual ~Device(){}

	virtual GpuBuffer*		newBuffer(const BufferDesc& desc) = 0;
	virtual Texture*		newTexture(const TextureDesc& desc) = 0;
	virtual RenderTarget*	newRenderWindow(Window* hwnd) = 0;
	virtual RenderTarget*	newRenderTexture(Texture* rtv, Texture* dsv = NULL) = 0;
	virtual InputLayout*	newInputLayout(const InputElement* elements, size_t count) = 0;
	virtual Program*		newProgram() = 0;
	//virtual PipelineLayout*	newPipelineLayout(const PipelineLayoutDesc& desc) = 0;
	virtual Pipeline*		newPipeline(const ComputePipelineDesc& desc) = 0;
	virtual Pipeline*		newPipeline(const GraphicsPipelineDesc& desc) = 0;
	virtual DescriptorSet*	newDescriptorSet(Program* prog) = 0;
	virtual CommandBuffer*	newCommandBuffer() = 0;
	virtual CommandQueue*	newCommandQueue() = 0;

	GpuBuffer* newVertexBuffer(uint32_t stride, uint32_t counts, const void* data = NULL, RES_FLAG flags = RES_DEFAULT)
	{
		BufferDesc desc(BU_VERTEX, stride, counts, flags, data);
		return newBuffer(desc);
	}
	GpuBuffer* newIndexBuffer(IndexType type, uint32_t counts, const void* data = NULL, RES_FLAG flags = RES_DEFAULT)
	{
		uint32_t stride = type == INDEX16 ? sizeof(uint16_t) : sizeof(uint32_t);
		BufferDesc desc(BU_INDEX, stride, counts, flags, data);
		return newBuffer(desc);
	}
};

// 用于全局,枚举GPU
class CU_API Graphics : public Object
{
	DECLARE_RTTI(Graphics, Object, OBJ_ID_GRAPHICS);

public:
	virtual ~Graphics(){}

	// 必须提供一个
	virtual Device* getDevice() = 0;
	// 创建
	//virtual void newDevice() = 0;
};

CU_NS_END
