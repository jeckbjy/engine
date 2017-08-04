#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

class CUTE_OGL_API OGL_Buffer : public IBuffer
{
public:
	OGL_Buffer(const BufferDesc& desc);
	~OGL_Buffer();
    
    void init(const BufferDesc& desc);
    void term();

    bool map(void_ptr& result);
    void unmap();
    
	void bind();

	GLuint handle() const { return m_vbo; }
	GLuint target() const { return m_target; }
private:
	GLuint m_vbo;
	GLenum m_target;
};

CUTE_NS_END
