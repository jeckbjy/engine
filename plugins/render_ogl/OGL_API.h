#pragma once

#ifndef CU_OGL_LIB
#define CU_OGL_DLL
#endif

#if defined(CU_OGL_LIB)
#	define CU_OGL_API
#elif defined(CU_OGL_BUILD)
#	define CU_OGL_API CU_DECL_EXPORT
#else
#	define CU_OGL_API CU_DECL_IMPORT
#endif

// auto-link
#if defined(_MSC_VER) && !defined(CU_OGL_BUILD) && !defined(CU_OGL_UN_AUTOLINK)
#pragma comment(lib, "plugin_ogl.lib")
#endif

#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#endif

#include "API.h"
#include "Graphics.h"

#ifdef _WIN32
#	define GLEW_STATIC
#	include<GL/glew.h>
#	define CU_USE_VAO
#	define CU_USE_GLEW

#elif defined(__ANDROID__) || defined(__QNX__)
#	include<EGL/egl.h>
#	include<GLES2/gl2.h>
#	include<GLES2/gl2ext.h>

#	define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#	define glClearDepth glClearDepthf
#	define CU_OPENGL_ES

extern PFNGLBINDVERTEXARRAYOESPROC		glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSOESPROC	glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSOESPROC		glGenVertexArrays;
extern PFNGLISVERTEXARRAYOESPROC		glIsVertexArray;

#elif defined(__APPLE__)
//////////////////////////////////////////////////////////////////////////
#	include "TargetConditionals.h"
#	if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#	include <OpenGLES/ES2/gl.h>
#	include <OpenGLES/ES2/glext.h>

#	define glClearDepth glClearDepthf
#	define CU_OPENGL_ES
#	define CU_USE_VAO

#	define glBindVertexArray	glBindVertexArrayOES
#	define glDeleteVertexArrays glDeleteVertexArraysOES
#	define glGenVertexArrays	glGenVertexArraysOES
#	define glIsVertexArray		glIsVertexArrayOES

#	elif TARGET_OS_MAC
// ����ϵͳ
#	include <OpenGL/gl.h>
#	include <OpenGL/glext.h>

#	define CU_USE_VAO

#	define glBindVertexArray	glBindVertexArrayAPPLE
#	define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#	define glGenVertexArrays	glGenVertexArraysAPPLE
#	define glIsVertexArray		glIsVertexArrayAPPLE

#	endif
#endif

// http://www.luluathena.com/?p=1739
// http://www.cnblogs.com/vertexshader/articles/3022981.html
//#	define CU_USE_SPO	// Separate Program Object
#	define CU_USE_PBO	// Pixel Buffer Object
#	define CU_USE_TEXTURE_ARRAY
#	define CU_USE_DRAW_INDEX_BASE
#	define CU_USE_UNIFORM_BLOCK
// �Ͱ汾��opengl
#	define GU_GL_ES2