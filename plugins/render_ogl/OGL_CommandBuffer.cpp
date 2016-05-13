#include "OGL_CommandBuffer.h"
#include "OGL_Mapping.h"
#include "OGL_Buffer.h"
#include "OGL_Program.h"
#include "OGL_InputLayout.h"
#include "OGL_Pipeline.h"

CU_NS_BEGIN

OGLCommandBuffer::OGLCommandBuffer()
: m_target(NULL)
, m_pipeline(NULL)
, m_layout(NULL)
, m_index(NULL)
, m_descriptors(NULL)
, m_primitive(PT_TRIANGLE_LIST)
, m_vertexCount(0)
, m_vertexStart(0)
, m_indexStart(0)
, m_indexCount(0)
, m_instanceCount(0)
{
}

OGLCommandBuffer::~OGLCommandBuffer()
{

}

void OGLCommandBuffer::reset()
{

}

void OGLCommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	glViewport(x, y, w, h);
}

void OGLCommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{

}

void OGLCommandBuffer::setBlendFactor(const float factors[4])
{
	memcpy(m_factors, factors, sizeof(m_factors));
}

void OGLCommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	m_stencilMask = mask;
	m_stencilRef = reference;
}

void OGLCommandBuffer::setRenderTarget(RenderTarget* target)
{
	m_target = target;
}

void OGLCommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{
	m_descriptors = descriptors;
}

void OGLCommandBuffer::setPipeline(Pipeline* pipeline)
{
	m_pipeline = (OGLPipeline*)pipeline;
}

void OGLCommandBuffer::setInputLayout(InputLayout* layout)
{
	m_layout = (OGLInputLayout*)layout;
}

void OGLCommandBuffer::setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets)
{

}

void OGLCommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{

}

void OGLCommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{

}

void OGLCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{

}

void OGLCommandBuffer::dispatch(size_t x, size_t y, size_t z)
{

}

//void OGLCommandBuffer::setIndexBuffer(IndexBuffer* ib)
//{
//	m_index = (OGLBuffer*)ib;
//}
//
//void OGLCommandBuffer::draw(const DrawParam& params)
//{
//	m_primitive = params.type;
//	m_vertexStart = params.vertexStart;
//	m_vertexCount = params.vertexCount;
//	m_indexStart = params.indexStart;
//	m_indexCount = params.indexCount;
//	m_instanceCount = params.instanceCount;
//}
//
//void OGLCommandBuffer::dispatch(size_t group_x, size_t group_y, size_t group_z)
//{
//
//}

void OGLCommandBuffer::execute()
{
	////Instance渲染  http://www.zwqxin.com/archives/opengl/talk-about-geometry-instancing.html
	////详细介绍:  http://book.2cto.com/201412/48549.html
	//GLint mode = OGLMapping::getPrimitiveMode(m_primitive);
	//// 绑定shader
	//OGLProgram* prog = (OGLProgram*)m_pipeline->getProgram();
	//prog->bind(m_descriptors);
	//m_layout->bind(prog);
	//if (m_vertexCount == 0)
	//{
	//	m_vertexCount = m_index ? m_index->count() : m_layout->getVertexCount();
	//}

	//if (m_instanceCount < 1)
	//{
	//	if (m_index != NULL && m_indexCount > 0)
	//	{
	//		m_index->bind();
	//		GLenum gl_type = m_index->isIndex16() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	//		// count指顶点个数
	//		glDrawElements(mode, m_vertexCount, gl_type, 0);
	//	}
	//	else
	//	{
	//		// 注：参数count是Vertex顶点个数
	//		glDrawArrays(mode, m_vertexStart, m_vertexCount);
	//	}
	//}
	//else
	//{
	//	// DrawInstance
	//	GLenum gl_type;
	//	size_t index_size;
	//	if (m_index->isIndex16())
	//	{
	//		gl_type = GL_UNSIGNED_SHORT;
	//		index_size = sizeof(unsigned short);
	//	}
	//	else
	//	{
	//		gl_type = GL_UNSIGNED_INT;
	//		index_size = sizeof(unsigned int);
	//	}
	//	bool gl3Support = false;
	//	if (gl3Support)
	//		glDrawElementsInstanced(mode, m_indexCount, gl_type, (const GLvoid*)(m_indexStart * index_size), m_instanceCount);
	//	else
	//		glDrawElementsInstancedARB(mode, m_indexCount, gl_type, (const GLvoid*)(m_indexStart * index_size), m_instanceCount);
	//}
}

CU_NS_END