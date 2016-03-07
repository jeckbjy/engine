#pragma once
#include "Window.h"
#include "GraphicsDesc.h"
#include "VertexLayout.h"

CU_NS_BEGIN

struct Box
{
	uint left;
	uint top;
	uint front;
	uint bottom;
	uint back;
};

struct Range
{
	uint begin;
	uint end;
};

class CU_API GpuResource : public Object
{
	DECLARE_RTTI(GpuResource, Object, "GRES");
public:
	virtual ~GpuResource() {}
	//virtual void map(uint subresource, const Range& pReadRange, void** data) = 0;
	//virtual void unmap() = 0;
};

class CU_API GpuBuffer : public GpuResource
{
	DECLARE_RTTI(GpuBuffer, GpuResource, "GBUF");
public:
	GpuBuffer(const BUFFER_DESC& desc);
	virtual ~GpuBuffer();

	virtual void* map(MAP_FLAG access, size_t offset, size_t len) = 0;
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

typedef SharedPtr<VertexBuffer>		VertexBufferPtr;
typedef SharedPtr<IndexBuffer>		IndexBufferPtr;
typedef SharedPtr<UniformBuffer>	UniformBufferPtr;

class CU_API Texture : public GpuResource
{
	DECLARE_RTTI(Texture, GpuResource, "GTEX");
public:
	Texture(const TEXTURE_DESC& desc){}
	virtual ~Texture(){}

	virtual void* map(PixelData& data, MAP_FLAG flag, uint level = 0, uint face = 0) = 0;
	virtual void unmap() = 0;

protected:
	PixelFormat m_format;
	TexType		m_type;
	RES_FLAG	m_usage;
	size_t		m_width;
	size_t		m_height;
	size_t		m_depth;
	size_t		m_mipmaps;
};

class CU_API RenderTarget : public Object
{
	DECLARE_RTTI(RenderTarget, Object, "RTAR");
public:
	virtual ~RenderTarget(){}
	virtual void present() = 0;
	virtual void clear(uint32_t flags = CLEAR_ALL, const Color& color = Color::BLACK, float depth = 1.0f, int32_t stencil = 0) = 0;
};

class CU_API Program : public Object
{
	DECLARE_RTTI(Program, Object, "PROG");
public:
	virtual ~Program(){}
	virtual bool attach(ShaderType type, const String& data, const String& name = "", const String& entry = "", ShaderProfile profile = SP_NONE, bool binary = false) = 0;
	virtual bool link() = 0;
};

// ������
class CU_API DescriptorSet : public Object
{
	DECLARE_RTTI(DescriptorSet, Object, "DESS");
public:
	virtual ~DescriptorSet(){}
	virtual void bind(const String& name, GpuResource* res) = 0;
};

// ��Ⱦ����,�������
class CU_API Pipeline : public Object
{
	DECLARE_RTTI(Pipeline, Object, "PPLE");
protected:
	Program* m_prog;
	Pipeline(const PIPELINE_DESC& desc);
	virtual ~Pipeline(){}

public:
	Program* getProgram() { return m_prog; }
};

// ���Ʋ���
struct CU_API DrawParam
{
	Topology	type;
	size_t		vertexStart;
	size_t		vertexCount;
	size_t		indexStart;
	size_t		indexCount;
	size_t		instanceCount;
	DrawParam() 
		: type(PT_TRIANGLE_LIST)
		, vertexStart(0)
		, vertexCount(0)
		, indexStart(0)
		, indexCount(0)
		, instanceCount(0)
	{}

	bool isInstanced() const { return instanceCount > 0; }
	bool isIndexed() const { return indexCount > 0; }
};

// execute buffer
class CU_API CommandBuffer : public Object
{
	DECLARE_RTTI(CommandBuffer, Object, "CMDB");
public:
	virtual ~CommandBuffer(){}

	virtual void setViewport(int x, int y, size_t w, size_t h) = 0;
	virtual void setBlendFactor(const float factors[4]) = 0;
	virtual void setStencilRef(size_t stencil) = 0;
	virtual void setRenderTarget(RenderTarget* target) = 0;
	virtual void setDescriptorSet(DescriptorSet* descriptors) = 0;
	virtual void setPipeline(Pipeline* pipeline) = 0;
	virtual void setVertexLayout(VertexLayout* layouts) = 0;
	virtual void setIndexBuffer(IndexBuffer* buffer) = 0;
	virtual void draw(const DrawParam& desc) = 0;
	virtual void dispatch(size_t group_x, size_t group_y, size_t group_z) = 0;
};

// ����
class CU_API CommandQueue : public Object
{
	DECLARE_RTTI(CommandQueue, Object, "CMDQ");
public:
	virtual ~CommandQueue(){}
	virtual void execute(CommandBuffer* cmds) = 0;
};

// �ײ���Ⱦ�ӿ�,mean Device create objs
class CU_API Device : public Object
{
	DECLARE_RTTI(Device, Object, "GRCS");
public:
	virtual ~Device(){}

	virtual GpuBuffer*		newBuffer(const BUFFER_DESC& desc) = 0;
	virtual Texture*		newTexture(const TEXTURE_DESC& desc) = 0;
	virtual RenderTarget*	newRenderWindow(Window* hwnd) = 0;
	virtual RenderTarget*	newRenderTexture(Texture* rtv, Texture* dsv = NULL) = 0;
	virtual VertexLayout*	newVertexLayout(VertexDeclaration& desc) = 0;
	virtual Program*		newProgram() = 0;
	virtual Pipeline*		newPipeline(const PIPELINE_DESC& desc) = 0;
	virtual DescriptorSet*	newDescriptorSet(Program* prog) = 0;
	virtual CommandBuffer*	newCommandBuffer() = 0;
	virtual CommandQueue*	newCommandQueue() = 0;
};

CU_NS_END
