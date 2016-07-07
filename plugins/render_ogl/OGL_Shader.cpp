#include "OGL_Shader.h"

CU_NS_BEGIN

GLuint mapping(ShaderType type)
{
	switch (type)
	{
	case SHADER_VERTEX:
		return GL_VERTEX_SHADER;
	case SHADER_HULL:
		return GL_TESS_CONTROL_SHADER;
	case SHADER_DOMAIN:
		return GL_TESS_EVALUATION_SHADER;
	case SHADER_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	case SHADER_PIXEL:
		return GL_FRAGMENT_SHADER;
	case SHADER_COMPUTE:
		return GL_COMPUTE_SHADER;
	default:
		return 0;
	}
}

bool is_sampler(GLint type)
{
	switch (type)
	{
	case GL_SAMPLER_1D:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_3D:
	case GL_SAMPLER_CUBE:
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////
//// Program
////////////////////////////////////////////////////////////////////////////
OGL_Shader::OGL_Shader(uint32_t id)
	: ShaderStage(id)
	, m_handle(0)
{
}

OGL_Shader::~OGL_Shader()
{
	destroy();
}

void OGL_Shader::destroy()
{
	if (m_handle != 0)
	{
		DeleteProgram(m_handle);
		m_handle = 0;
	}
}

bool OGL_Shader::compile(const ShaderDesc& desc)
{
	destroy();

	GLint handle = 0;
	const char* source = desc.code.c_str();
	GLenum	gltype = mapping(desc.stage);
	bool	has_error;
#ifdef CU_USE_SPO
	handle = glCreateShaderProgramv(gltype, 1, &source);
	has_error = (glGetError() != GL_NO_ERROR);
#else
	GLint status = 0;
	handle = glCreateShader(gltype);
	glShaderSource(handle, 1, &source, NULL);
	glCompileShader(handle);
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	has_error = (status != GL_TRUE);
#endif

	// 打印错误信息
	if (has_error)
	{
		GLsizei len;
		String log;
		GetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
		if (len != 0)
		{
			log.resize(len);
			GetShaderInfoLog(handle, len, NULL, &log[0]);
			log[len - 1] = '\0';
		}
		// 释放资源
		if (handle != 0)
		{
			DeleteShader(handle);
			handle = 0;
		}

		if (!log.empty())
			throw std::runtime_error(log);

		return false;
	}

	m_handle = handle;
	return true;
}

CU_NS_END