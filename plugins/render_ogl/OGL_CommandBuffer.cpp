#include "OGL_CommandBuffer.h"
#include "OGL_Mapping.h"
#include "OGL_Buffer.h"
#include "OGL_Program.h"
#include "OGL_InputLayout.h"
#include "OGL_Pipeline.h"

CU_NS_BEGIN

OGL_CommandBuffer::OGL_CommandBuffer()
	: m_target(NULL)
	, m_pipeline(NULL)
	, m_layout(NULL)
	, m_index(NULL)
	, m_descriptors(NULL)
	, m_primitive(GL_TRIANGLES)
	, m_vertexCount(0)
	, m_vertexStart(0)
	, m_indexStart(0)
	, m_indexCount(0)
	, m_instanceCount(0)
{
	for (int i = 0; i < CU_MAX_VERTEX_BUFFERS; ++i)
	{
		m_vertices[i] = NULL;
	}
}

OGL_CommandBuffer::~OGL_CommandBuffer()
{

}

void OGL_CommandBuffer::setRenderTarget(RenderTarget* target)
{
	m_target = target;
	m_target->bind(NULL);
}

void OGL_CommandBuffer::setViewport(int x, int y, size_t w, size_t h)
{
	glViewport(x, y, w, h);
}

void OGL_CommandBuffer::setScissor(int x, int y, size_t w, size_t h)
{

}

void OGL_CommandBuffer::setBlendFactor(const float factors[4])
{
	memcpy(m_factors, factors, sizeof(m_factors));
}

void OGL_CommandBuffer::setStencilRef(StencilFaceFlags mask, size_t reference)
{
	m_stencilMask = mask;
	m_stencilRef = reference;
}

void OGL_CommandBuffer::setTopology(Topology topology)
{
	m_primitive = OGL_Mapping::getPrimitiveMode(topology);
}

void OGL_CommandBuffer::setDescriptorSet(DescriptorSet* descriptors)
{
	m_descriptors = descriptors;
}

void OGL_CommandBuffer::setPipeline(Pipeline* pipeline)
{
	m_pipeline = (OGL_Pipeline*)pipeline;
}

void OGL_CommandBuffer::setInputLayout(InputLayout* layout)
{
	m_layout = (OGL_InputLayout*)layout;
}

void OGL_CommandBuffer::setVertexBuffers(size_t startSlot, size_t counts, GpuBuffer** buffers, size_t* offsets)
{
	m_verticeStart = startSlot;
	m_verticeCount = counts;
	for (size_t i = 0; i < counts; ++i)
	{
		m_vertices[startSlot + i] = (OGL_Buffer*)buffers[i];
	}
}

void OGL_CommandBuffer::setIndexBuffer(IndexBuffer* buffer, size_t offset)
{
	m_index = (OGL_Buffer*)buffer;
}

void OGL_CommandBuffer::clear(ClearMask mask, const Color& color, float depth, uint32_t stencil, uint8_t targetMask)
{
	if (mask & CLEAR_COLOR)
	{
		glColorMask(true, true, true, true);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	if (mask & CLEAR_DEPTH)
	{
		glDepthMask(GL_TRUE);
		glClearDepth(depth);
	}

	if (mask & CLEAR_STENCIL)
	{
		glStencilMask(0xFFFFFFFF);
		glClearStencil(stencil);
	}
}

void OGL_CommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexOffset, uint32_t instanceOffset)
{
	prepare();

	// todo:how set instanceOffset
	if (instanceCount <= 1)
		glDrawArrays(m_primitive, vertexOffset, vertexCount);
	else
		glDrawArraysInstanced(m_primitive, vertexOffset, vertexCount, instanceCount);
}

void OGL_CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexOffset, uint32_t instanceOffset, uint32_t vertexOffset)
{
	if (!m_index)
		return;

	prepare();
	// bind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index->handle());
	GLenum indexType = m_index->isIndex16() ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

	// 版本校验,低版本
#if OGL_VERSION <= OGL_VERSION_2
	glDrawElements(m_primitive, indexCount, indexType, NULL);
#else
	if (instanceCount <= 1)
	{
		glDrawElementsBaseVertex(m_primitive, indexCount, indexType, (void*)indexOffset, vertexOffset);
	}
	else
	{
		// todo: set instanceOffset
		glDrawElementsInstancedBaseVertex(m_primitive, indexCount, indexType, (void*)indexOffset, instanceCount, vertexOffset);
	}
#endif

}

void OGL_CommandBuffer::dispatch(size_t x, size_t y, size_t z)
{
#if (OGL_VERSION >= OGL_VERSION_4)
	if (m_pipeline)
		m_pipeline->bind();

	glDispatchCompute(x, y, z);
#endif
}

void OGL_CommandBuffer::prepare()
{
	if (m_pipeline)
		m_pipeline->bind();

	if (m_layout)
	{
		m_layout->bind();
	}
}

CU_NS_END