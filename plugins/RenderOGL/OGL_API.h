#pragma once

// define CUTE_OGL_API
#if defined(CUTE_USE_LIB_OGL)
// build or use lib
#	define CUTE_OGL_API
#elif defined(CUTE_BUILD_OGL)
// build dynamic dll
#	define CUTE_OGL_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_OGL_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_OGL) && !defined(CUTE_UNAUTOLINK_OGL)
#	pragma comment(lib, "plugin_ogl.lib")
#endif

#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#endif

#include "Cute/RenderAPI.h"

//
#define OGL_VERSION_2	2
#define OGL_VERSION_3	3
#define OGL_VERSION_4	4

#ifdef _WIN32
#	define GLEW_STATIC
#	include<GL/glew.h>
#	define CU_USE_GLEW
#	define CU_HAS_VAO
#	define CU_HAS_SPO

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
#	define CUTE_OPENGL_ES
#	define CUTE_HAS_VAO

#	define glBindVertexArray	glBindVertexArrayOES
#	define glDeleteVertexArrays glDeleteVertexArraysOES
#	define glGenVertexArrays	glGenVertexArraysOES
#	define glIsVertexArray		glIsVertexArrayOES

#	elif TARGET_OS_MAC
#	include <OpenGL/gl3.h>
#	include <OpenGL/gl3ext.h>

#	define CUTE_HAS_VAO

#	define glBindVertexArray	glBindVertexArrayAPPLE
#	define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#	define glGenVertexArrays	glGenVertexArraysAPPLE
#	define glIsVertexArray		glIsVertexArrayAPPLE

#	endif
#endif

// http://www.luluathena.com/?p=1739
// http://www.cnblogs.com/vertexshader/articles/3022981.html
#define CUTE_USE_PBO	// Pixel Buffer Object
#define CUTE_USE_TEXTURE_ARRAY
#define CUTE_USE_DRAW_INDEX_BASE
#define CUTE_USE_UNIFORM_BLOCK

#define OGL_VERSION		OGL_VERSION_4

#if defined(CU_HAS_VAO) && !defined(CU_NO_VAO)
#define CUTE_USE_VAO
#endif

#if defined(CU_HAS_SPO) && !defined(CU_NO_SPO)
#define CUTE_USE_SPO
#endif
