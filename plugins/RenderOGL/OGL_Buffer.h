#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

// todo：uniform buf需要延迟创建,用时更新,因为上层不一定使用
class CU_OGL_API OGL_Buffer : public GpuBuffer
{
public:
	OGL_Buffer(const BufferDesc& desc);
	~OGL_Buffer();

	void* map(size_t offset, size_t len, MAP_FLAG access);
	void unmap();

	void bind();

	GLuint handle() const { return m_vbo; }
	GLuint target() const { return m_target; }
private:
	GLuint m_vbo;		// buffer
	GLenum m_target;	// 目标
};

CUTE_NS_END
