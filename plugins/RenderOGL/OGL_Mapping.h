#pragma once
#include "OGL_API.h"

CUTE_NS_BEGIN

struct CUTE_OGL_API OGL_Mapping
{
	static void         getFormat(RESOURCE_FORMAT fmt, GLint& glinternal, GLenum& glformat, GLenum& gltype);
	static GLenum       getGLInternal(RESOURCE_FORMAT fmt, bool gamma = false);
	static GLenum       getGLFormat(RESOURCE_FORMAT fmt);
	static GLenum       getGLType(RESOURCE_FORMAT fmt);
    
    static GLenum       getUsage(RESOURCE_USAGE usage);
//	static GLenum       getBufferUsage(BufferUsage type);
//	static GLint        getPrimitiveMode(Topology type);
//    static GLbitfield   getAccess(MAP_FLAG type);

//	static UniformType getUniformType(GLenum type);
};

CUTE_NS_END
