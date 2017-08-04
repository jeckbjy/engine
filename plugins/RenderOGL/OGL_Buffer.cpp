#include "OGL_Buffer.h"
#include "OGL_Mapping.h"

CUTE_NS_BEGIN

OGL_Buffer::OGL_Buffer(const BufferDesc& desc)
    : IBuffer(desc)
    , m_vbo(0)
    , m_target(0)
{
}

OGL_Buffer::~OGL_Buffer()
{
    term();
}

void OGL_Buffer::init(const BufferDesc &desc)
{
    bool isDyanmic = (desc.usage & RESOURCE_USAGE_DYNAMIC) != 0;
    GLenum usage = isDyanmic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    m_target = OGL_Mapping::getUsage(desc.usage);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(m_target, m_vbo);
    
    if(desc.data != NULL)
        glBufferData(m_target, desc.size, desc.data, usage);
}

void OGL_Buffer::term()
{
    if(m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
}

bool OGL_Buffer::map(void_ptr& result)
{
//	GLenum access = OGL_Mapping::getAccess(flag);
    GLenum access = GL_MAP_WRITE_BIT;
    GLintptr offset = 0;
    GLsizeiptr length = getSize();
    
	glBindBuffer(m_target, m_vbo);
	result = glMapBufferRange(m_target, offset, length, access);
    return result != NULL;
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

CUTE_NS_END
