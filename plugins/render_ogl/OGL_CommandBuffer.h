#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

class OGLProgram;
class OGLPipeline;
class OGLInputLayout;
class OGLBuffer;
class CU_OGL_API OGLCommandBuffer : public CommandBuffer
{
public:
	OGLCommandBuffer();
	~OGLCommandBuffer();

	void setViewport(int x, int y, size_t w, size_t h);
	void setScissor(int x, int y, size_t w, size_t h);
	void setBlendFactor(const float factors[4]);
	void setStencilRef(StencilFaceFlags mask, size_t reference);
	void setRenderTarget(RenderTarget* target);
	void setDescriptorSet(DescriptorSet* descriptors);
	void setPipeline(Pipeline* pipeline);
	void setInputLayout(InputLayout* layout);
	void setIndexBuffer(IndexBuffer* ib);

	void draw(const DrawParam& params);
	void dispatch(size_t group_x, size_t group_y, size_t group_z);

	void execute();	// 执行
private:
	RenderTarget*		m_target;
	OGLPipeline*		m_pipeline;
	OGLInputLayout*		m_layout;
	OGLBuffer*			m_index;
	DescriptorSet*		m_descriptors;
	Topology			m_primitive;
	size_t				m_vertexStart;
	size_t				m_vertexCount;	// 顶点数目
	size_t				m_indexStart;
	size_t				m_indexCount;
	size_t				m_instanceCount;

	float				m_factors[4];
	StencilFaceFlags	m_stencilMask;
	size_t				m_stencilRef;
};

class CU_OGL_API OGLCommandQueue : public CommandQueue
{
public:
	void execute(CommandBuffer* cmd){ ((OGLCommandBuffer*)cmd)->execute();  }
};

CU_NS_END