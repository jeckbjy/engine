#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class OGLProgram;
class OGLPipeline;
class OGLVertexLayout;
class OGLBuffer;
class CU_OGL_API OGLCommandBuffer : public CommandBuffer
{
public:
	OGLCommandBuffer();
	~OGLCommandBuffer();

	void setViewport(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(size_t stencil);
	void setRenderTarget(RenderTarget* target);
	void setDescriptorSet(DescriptorSet* descriptors);
	void setTopology(Topology primitive);
	void setPipeline(Pipeline* pipeline);
	void setVertexLayout(VertexLayout* layout);
	void setIndexBuffer(IndexBuffer* ib);

	virtual void draw(size_t vnum, size_t voff, size_t instance_num, size_t instance_off);
	virtual void drawIndexed(size_t inum, size_t ioff, size_t instance_num, size_t instance_off, int vertex_base);
	virtual void dispatch(size_t group_x, size_t group_y, size_t group_z);

	void execute();	// 执行
private:
	RenderTarget*		_target;
	OGLPipeline*		_pipeline;
	OGLVertexLayout*	_layout;
	OGLBuffer*			_ib;
	DescriptorSet*		_descriptors;
	Topology			_primitive;
	size_t				_verticesNum;	// 顶点数目
	size_t				_verticesOff;
	size_t				_instanceNum;
	size_t				_instanceOff;
	size_t				_vertexBase;	// index mode

	float	_factors[4];
	size_t  _stencil_ref;
};

class CU_OGL_API OGLCommandQueue : public CommandQueue
{
public:
	void execute(CommandBuffer* cmd){ ((OGLCommandBuffer*)cmd)->execute();  }
};

CU_NS_END