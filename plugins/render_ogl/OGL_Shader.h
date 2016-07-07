#pragma once
#include "OGL_API.h"

#ifdef CU_USE_SPO
#define CreateProgram(h)	glGenProgramPipelines(1, &h)
#define DeleteProgram(h)	glDeleteProgramPipelines(1, &h)
#define UseProgram(h)		glBindProgramPipeline(h)
// shader object
#define DeleteShader(h)		glDeleteProgram(h)
#define GetShaderiv			glGetProgramiv
#define GetShaderInfoLog	glGetProgramInfoLog
#else
#define CreateProgram(h)	h = glCreateProgram()
#define DeleteProgram(h)	glDeleteProgram(h)
#define UseProgram(h)		glUseProgram(h)
// shader object
#define DeleteShader(h)		glDeleteShader(h)
#define GetShaderiv			glGetShaderiv
#define GetShaderInfoLog	glGetShaderInfoLog
#endif

CU_NS_BEGIN

class CU_OGL_API OGL_Shader : public ShaderStage
{
public:
	OGL_Shader(uint32_t id);
	~OGL_Shader();

	bool compile(const ShaderDesc& desc);

	GLuint native() { return m_handle; }
private:
	void destroy();

private:
	GLuint m_handle;
};

CU_NS_END