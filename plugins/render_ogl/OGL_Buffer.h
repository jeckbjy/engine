#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

// todo：uniform buf需要延迟创建,用时更新,因为上层不一定使用
class CU_OGL_API OGLBuffer : public GpuBuffer
{
public:
	OGLBuffer(const BufferDesc& desc);
	~OGLBuffer();

	void* map(MAP_FLAG access, uint32_t offset, uint32_t len);
	void unmap();

	void bind();

	GLuint handle() const { return m_vbo; }
	GLuint target() const { return m_target; }
private:
	GLuint m_vbo;		// buffer
	GLenum m_target;	// 目标
};

CU_NS_END