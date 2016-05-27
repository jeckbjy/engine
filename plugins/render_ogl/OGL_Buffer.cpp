#include "OGL_Buffer.h"
#include "OGL_Mapping.h"

CU_NS_BEGIN

OGL_Buffer::OGL_Buffer(const BufferDesc& desc)
:GpuBuffer(desc)
{
	GLenum usage = isDynamic()? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	// 创建并分配空间
	m_target = OGL_Mapping::getBufferUsage(desc.usage);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(m_target, m_vbo);
	glBufferData(m_target, m_bytes, desc.data, usage);
}

OGL_Buffer::~OGL_Buffer()
{
	glDeleteBuffers(1, &m_vbo);
}

void* OGL_Buffer::map(size_t offset, size_t len, MAP_FLAG flag)
{
	GLenum access = OGL_Mapping::getAccess(flag);
	glBindBuffer(m_target, m_vbo);
	void* buff = glMapBufferRange(m_target, offset, len, access);
	if(buff == 0)
		throw std::runtime_error("index buffer:Out of memory");
	return buff;
}

void OGL_Buffer::unmap()
{
	glBindBuffer(m_target, m_vbo);
	if(!glUnmapBuffer(m_target))
		throw std::runtime_error("OGLGpuBuffer unlock error:Buffer data corrupted, please reload");
}

void OGL_Buffer::bind()
{
	glBindBuffer(m_target, m_vbo);
}

CU_NS_END