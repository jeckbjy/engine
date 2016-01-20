#pragma once
#include "GpuBuffer.h"
#include "Texture.h"

CU_NS_BEGIN
//
//class CU_API RenderTarget : public Ref
//{
//public:
//	virtual ~RenderTarget(){}
//	virtual void present() = 0;
//	virtual void clear(uint32_t flags = CLEAR_ALL, const Color& color = Color::Black, float depth = 1.0f, int32_t stencil = 0) = 0;
//};
//
//// 顶点结构
//class CU_API VertexLayout : public Ref
//{
//public:
//	VertexLayout(VertexDeclaration& desc);
//	virtual ~VertexLayout(){}
//
//	void setBuffer(VertexBuffer* vb, int slot = 0);
//	void setStart(size_t off) { _start = off; }
//
//protected:
//	typedef std::map<int, VertexBuffer*> BufferList;
//	bool		_dirty;
//	BufferList	_buffers;
//	size_t		_start;
//};
//
//class Program : public Ref
//{
//public:
//	virtual ~Program(){}
//	virtual bool attach(ShaderType type, const String& data, const String& name = "", const String& entry = "", ShaderProfile profile = SP_NONE, bool binary = false) = 0;
//	virtual bool link() = 0;
//};
//
//// 描述符
//class CU_API DescriptorSet : public Ref
//{
//public:
//	virtual ~DescriptorSet(){}
//	virtual void bind(const String& name, Resource* res) = 0;
//};
//
//// 渲染管线,计算管线
//class CU_API Pipeline : public Ref
//{
//protected:
//	Program* _prog;
//	Pipeline(const PipelineDesc& desc) :_prog(desc.program){}
//	virtual ~Pipeline(){}
//
//public:
//	Program* getProgram() { return _prog; }
//};

// execute buffer
class CU_API CommandBuffer : public Object
{
	DECLARE_RTTI(CommandBuffer, Object, "CMDB");
public:
	virtual ~CommandBuffer(){}
	// other set
};

class CU_API CommandQueue : public Object
{
	DECLARE_RTTI(CommandQueue, Object, "CMDQ");
public:
	virtual ~CommandQueue(){}
	virtual void execute(CommandBuffer* cmds) = 0;
};

// 底层渲染接口,mean Device create objs
class CU_API Grahpics : public Object
{
	DECLARE_RTTI(Grahpics, Object, "GRCS");
public:
	virtual void draw() = 0;
	virtual void drawInstance() = 0;
	virtual void dispatch(int x, int y, int z) = 0;
};

CU_NS_END
