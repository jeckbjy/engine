#pragma once
#include "OGL_API.h"

CU_NS_BEGIN

struct CU_OGL_API OGL_Mapping
{
	static void getPixelFormat(PixelFormat fmt, GLint& glinternal, GLenum& glformat, GLenum& gltype);
	static GLenum getGLInternal(PixelFormat fmt, bool gamma = false);
	static GLenum getGLFormat(PixelFormat fmt);
	static GLenum getGLType(PixelFormat fmt);
	static GLenum getBufferUsage(BufferUsage type);
	static GLbitfield getAccess(MAP_FLAG type);
	static GLint getPrimitiveMode(Topology type);

	static UniformType getUniformType(GLenum type);
};

CU_NS_END