#include "OGL_Buffer.h"
#include "OGL_Mapping.h"

CU_NS_BEGIN

OGLBuffer::OGLBuffer(const BUFFER_DESC& desc)
:GpuBuffer(desc)
{
	GLenum usage = isDynamic()? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	// 创建并分配空间
	m_target = OGLMapping::getBinding(desc.type);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(m_target, m_vbo);
	glBufferData(m_target, m_bytes, desc.data, usage);
}

OGLBuffer::~OGLBuffer()
{
	glDeleteBuffers(1, &m_vbo);
}

void* OGLBuffer::map(MAP_FLAG flag, uint32_t offset, uint32_t len)
{
	GLenum access = OGLMapping::getAccess(flag);
	glBindBuffer(m_target, m_vbo);
	void* buff = glMapBufferRange(m_target, offset, len, access);
	if(buff == 0)
		throw std::runtime_error("index buffer:Out of memory");
	return buff;
}

void OGLBuffer::unmap()
{
	glBindBuffer(m_target, m_vbo);
	if(!glUnmapBuffer(m_target))
		throw std::runtime_error("OGLGpuBuffer unlock error:Buffer data corrupted, please reload");
}

void OGLBuffer::bind()
{
	glBindBuffer(m_target, m_vbo);
}

CU_NS_END