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

//OGLProgram::OGLProgram()
//	:m_handle(0)
//{
//	memset(m_attrs, 0, sizeof(m_attrs));
//	CreateProgram(m_handle);
//}
//
//OGLProgram::~OGLProgram()
//{
//	// 释放shader,目前直接释放，todo:引用计数重复利用
//	for (ShaderMap::iterator itor = m_shaders.begin(); itor != m_shaders.end(); ++itor)
//	{
//		DeleteShader(itor->second);
//	}
//	m_shaders.clear();
//
//	if (m_handle != 0)
//	{
//		DeleteProgram(m_handle);
//		m_handle = 0;
//	}
//}

//bool OGLProgram::attach(ShaderType type, const String& data, const String& name, const String& entry, ShaderProfile profile, bool binary)
//{
//	// 先查找
//	GLuint handle = binary ? load(type, data) : compile(type, data);
//	if (handle != 0)
//	{
//#ifdef CU_USE_SPO
//		glUseProgramStages(m_handle, mapping_bit(type), handle);
//#else
//		glAttachShader(m_handle, handle);
//#endif
//		m_shaders[type] = handle;
//		return true;
//	}
//
//	return false;
//}
//
//GLuint OGLProgram::compile(ShaderType type, const String& data)
//{
//	GLuint handle;
//
//	const char* source = data.c_str();
//	GLenum	gltype = mapping(type);
//	bool	has_error;
//#ifdef CU_USE_SPO
//	handle = glCreateShaderProgramv(gltype, 1, &source);
//	has_error = (glGetError() != GL_NO_ERROR);
//#else
//	GLint status = 0;
//	handle = glCreateShader(gltype);
//	glShaderSource(handle, 1, &source, NULL);
//	glCompileShader(handle);
//	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
//	has_error = (status != GL_TRUE);
//#endif
//
//	// 打印错误信息
//	if (has_error)
//	{
//		GLsizei len;
//		String log;
//		GetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
//		if (len != 0)
//		{
//			log.resize(len);
//			GetShaderInfoLog(handle, len, NULL, &log[0]);
//			log[len - 1] = '\0';
//		}
//		// 释放资源
//		if (handle != 0)
//		{
//			DeleteShader(handle);
//			handle = 0;
//		}
//
//		if (!log.empty())
//			throw std::runtime_error(log);
//	}
//
//	return handle;
//}
//
//GLuint OGLProgram::load(ShaderType type, const String& data)
//{
//	return 0;
//}

//bool OGLProgram::link()
//{
//	bool successed = true;
//#ifdef CU_USE_SPO
//	ShaderMap::iterator itor = m_shaders.find(ST_VERTEX);
//	if (itor != m_shaders.end() && !parse_attrs(itor->second))
//		successed = true;
//	else
//		successed = false;
//	//todo: 对所有的shader解析uniform
//#else
//	GLint success;
//	glLinkProgram(m_handle);
//	glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
//	successed = (success == GL_TRUE);
//	if (!parse_attrs(m_handle))
//		successed = false;
//	if (!parse_uniform(m_handle))
//		successed = false;
//#endif
//	return successed;
//}

//bool OGLProgram::parse_attrs(GLuint handle)
//{
//	bool successed = true;
//	// 解析attr
//	GLint attr_num, len;
//	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &attr_num);
//	if (attr_num > 0)
//		glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &len);
//	if (attr_num > 0 && len > 0)
//	{
//		GLchar* name = new GLchar[len + 1];
//		GLint size;
//		GLenum type;
//		GLint location;
//		for (int i = 0; i < attr_num; ++i)
//		{
//			glGetActiveAttrib(handle, i, len, NULL, &size, &type, name);
//			name[len] = '\0';
//			location = glGetAttribLocation(handle, name);
//			// 设置
//			int index = find_usage(name);
//			// 映射，将名字映射成Position
//			if (index != -1)
//				m_attrs[index] = location;
//			else
//				successed = false;
//		}
//	}
//	return successed;
//}
//
//// 获取字符大小
//inline static GLint get_max_buf_size(GLuint handle)
//{
//	GLint buf_size;
//	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &buf_size);
//#ifdef CU_USE_UNIFORM_BLOCK
//	GLint block_name_size = 0;
//	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &block_name_size);
//	if (block_name_size > buf_size)
//		buf_size = block_name_size;
//#endif
//	return buf_size;
//}
//
//bool OGLProgram::parse_uniform(GLuint handle)
//{
//	Variable variable;
//	Uniform uniform;
//	UniformDesc desc;
//	GLint buf_size = get_max_buf_size(handle);
//	GLchar* name = new GLchar[buf_size];
//
//	// 解析Block
//#ifdef CU_USE_UNIFORM_BLOCK
//	// parse block
//	GLint block_size = 0;
//	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCKS, &block_size);
//	for (GLuint i = 0; i < (GLuint)block_size; ++i)
//	{
//		GLsizei block_size = 0;
//		glGetActiveUniformBlockName(handle, i, buf_size, &block_size, name);
//		//uniform.location = i;
//
//		uniform.name = name;
//		uniform.category = UT_BLOCK;
//		uniform.location = i;
//		uniform.size = block_size;
//		m_uniforms.push_back(uniform);
//		// 插入？
//		//desc.name = name;
//		//desc.size = block_size;
//		//_desc.uniforms.push_back(desc);
//	}
//#endif
//
//	uint sampler_index = 0;	// 累计索引
//	GLenum uni_type = 0;
//	GLint uni_size = 0;
//	GLint active_size = 0;
//	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &active_size);
//	for (GLuint index = 0; index < (GLuint)active_size; ++index)
//	{
//		glGetActiveUniform(handle, index, buf_size, NULL, &uni_size, &uni_type, name);
//		// If this is an array uniform, strip array indexers off it since GL does not
//		// seem to be consistent across different drivers/implementations in how it returns
//		// array uniforms. On some systems it will return "u_matrixArray", while on others
//		// it will return "u_matrixArray[0]".
//		char* c = strchr(name, '[');
//		if (c)
//			*c = '\0';
//
//#ifdef CU_USE_UNIFORM_BLOCK
//		GLint block_index = -1;
//		glGetActiveUniformsiv(handle, 1, &index, GL_UNIFORM_BLOCK_INDEX, &block_index);
//		if (block_index != -1)
//		{
//			GLint offset;
//			glGetActiveUniformsiv(handle, 1, &index, GL_UNIFORM_OFFSET, &offset);
//			// 设置
//			variable.name = name;
//			variable.block = block_index;
//			variable.offset = offset;
//			variable.size = 0;
//			m_variables.push_back(variable);
//			continue;
//		}
//#endif
//		// tex or value
//		uniform.name = name;
//		uniform.category = UT_BUFFER;
//		uniform.location = glGetUniformLocation(handle, name);
//		uniform.type = uni_type;
//		uniform.arrays = uni_size;
//		uniform.index = 0;
//		if (is_sampler(uni_type))
//		{
//			uniform.category = UT_TEXTURE;
//			uniform.index = sampler_index;
//			sampler_index += uni_size;
//		}
//
//		m_uniforms.push_back(uniform);
//	}
//
//	// 整理信息
//	for (size_t i = 0; i < m_uniforms.size(); ++i)
//	{
//		const Uniform& uni = m_uniforms[i];
//		ParamInfo info(PT_UNIFORM, i, 0, uni.size);
//		m_params[uni.name] = info;
//	}
//	// 变量信息
//	for (size_t i = 0; i < m_variables.size(); ++i)
//	{
//		const Variable& var = m_variables[i];
//		ParamInfo info(PT_VARIABLE, var.block, var.offset, var.size);
//		m_params[var.name] = info;
//	}
//
//	return true;
//}
//
//const ParamInfo* OGLProgram::getInfo(const String& name) const
//{
//	ParamMap::const_iterator itor = m_params.find(name);
//	if (itor != m_params.end())
//		return &(itor->second);
//	return NULL;
//}
//
//static void bind_uniform(const char* data, const Uniform& uni)
//{
//	GLint location = uni.location;
//	GLint array_size = uni.arrays;
//	switch (uni.type)
//	{
//	case GL_FLOAT:
//		glUniform1fv(location, array_size, (const GLfloat*)data);
//		break;
//	case GL_FLOAT_VEC2:
//		glUniform2fv(location, array_size, (const GLfloat*)data);
//		break;
//	case GL_FLOAT_VEC3:
//		glUniform3fv(location, array_size, (const GLfloat*)data);
//		break;
//	case GL_FLOAT_VEC4:
//		glUniform4fv(location, array_size, (const GLfloat*)data);
//		break;
//	case GL_INT:
//		glUniform1iv(location, array_size, (const GLint*)data);
//		break;
//	case GL_INT_VEC2:
//		glUniform2iv(location, array_size, (const GLint*)data);
//		break;
//	case GL_INT_VEC3:
//		glUniform3iv(location, array_size, (const GLint*)data);
//		break;
//	case GL_INT_VEC4:
//		glUniform4iv(location, array_size, (const GLint*)data);
//		break;
//	case GL_UNSIGNED_INT:
//		glUniform1uiv(location, array_size, (const GLuint*)data);
//		break;
//	case GL_UNSIGNED_INT_VEC2:
//		glUniform2uiv(location, array_size, (const GLuint*)data);
//		break;
//	case GL_UNSIGNED_INT_VEC3:
//		glUniform3uiv(location, array_size, (const GLuint*)data);
//		break;
//	case GL_UNSIGNED_INT_VEC4:
//		glUniform4uiv(location, array_size, (const GLuint*)data);
//		break;
//	case GL_BOOL:
//		break;
//	case GL_BOOL_VEC2:
//		break;
//	case GL_BOOL_VEC3:
//		break;
//	case GL_BOOL_VEC4:
//		break;
//	case GL_FLOAT_MAT2:
//		glUniformMatrix2fv(location, array_size, GL_FALSE, (const GLfloat*)data);
//		break;
//	case GL_FLOAT_MAT3:
//		glUniformMatrix3fv(location, array_size, GL_FALSE, (const GLfloat*)data);
//		break;
//	case GL_FLOAT_MAT4:
//		glUniformMatrix4fv(location, array_size, GL_FALSE, (const GLfloat*)data);
//		break;
//	case GL_FLOAT_MAT2x3:
//	case GL_FLOAT_MAT2x4:
//	case GL_FLOAT_MAT3x2:
//	case GL_FLOAT_MAT3x4:
//	case GL_FLOAT_MAT4x2:
//	case GL_FLOAT_MAT4x3:
//		// 目前不支持
//		assert(false);
//		break;
//	default:
//		break;
//	}
//}
//
//void OGLProgram::bind(DescriptorSet* descriptors)
//{
//	if (m_handle == 0)
//		return;
//	UseProgram(m_handle);
//	if (descriptors == NULL)
//		return;
//
//	OGLDescriptorSet* ogl_des = (OGLDescriptorSet*)descriptors;
//
//	size_t size = std::min(ogl_des->size(), m_uniforms.size());
//
//	for (size_t i = 0; i < size; ++i)
//	{
//		const Uniform& uni = m_uniforms[i];
//		const Descriptor* desc = ogl_des->at(i);
//		if (!desc)
//			continue;
//		if (desc->isKindOf<OGLTexture>())
//		{
//			OGLTexture* tex = (OGLTexture*)desc;
//			if (tex)
//				tex->active(uni.index);
//		}
//		else if (desc->isKindOf<OGLBuffer>())
//		{
//			OGLBuffer* gl_buf = (OGLBuffer*)desc;
//			// todo:shadow
//			//bind_uniform(gl_buf->getBuffer(), uni);
//
//#ifdef CU_USE_UNIFORM_BLOCK
//			GLuint uniformBlockBinding = 0;
//			glUniformBlockBinding(m_handle, uni.location, uniformBlockBinding);
//			glBindBufferRange(GL_UNIFORM_BUFFER, uniformBlockBinding, gl_buf->handle(), 0, gl_buf->bytes());
//#endif
//		}
//	}
//}
//
//void OGLProgram::unbind()
//{
//	UseProgram(0);
//}

CU_NS_END