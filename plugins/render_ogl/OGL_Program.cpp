#include "OGL_Program.h"
#include "OGL_Mapping.h"
#include "OGL_Texture.h"
#include "OGL_Buffer.h"
#include "OGL_DescriptorSet.h"

CU_NS_BEGIN

GLuint mapping_bit(uint8_t type)
{
	switch (type)
	{
	case SHADER_VERTEX:
		return GL_VERTEX_SHADER_BIT;
	case SHADER_HULL:
		return GL_TESS_CONTROL_SHADER_BIT;
	case SHADER_DOMAIN:
		return GL_TESS_EVALUATION_SHADER_BIT;
	case SHADER_GEOMETRY:
		return GL_GEOMETRY_SHADER_BIT;
	case SHADER_PIXEL:
		return GL_FRAGMENT_SHADER_BIT;
	case SHADER_COMPUTE:
		return GL_COMPUTE_SHADER_BIT;
	default:
		return 0;
	}
}

// 管理全局
int find_usage(const char* name)
{// 查找对应的usage
	static const char* gl_names[] =
	{
		"a_position", "a_normal", "a_color", "a_tangent", "a_binormal", "a_blendweights", "a_blendindices",
		"a_texcoord0", "a_texcoord1", "a_texcoord2", "a_texcoord3", "a_texcoord4", "a_texcoord5", "a_texcoord6", "a_texcoord7"
	};
	for (int i = SEMANTIC_POSITION; i < SEMANTIC_MAX; ++i)
	{
		if (strcmp(name, gl_names[i]) == 0)
			return i;
	}
	return -1;
}

OGL_Program::OGL_Program(uint32_t id)
	: ShaderProgram(id)
	, m_handle(0)
{
	CreateProgram(m_handle);
	for (int i = 0; i < SEMANTIC_MAX; ++i)
	{
		m_attrs[i] = -1;
	}
}

OGL_Program::~OGL_Program()
{
	// 释放
	UniformDesc* desc;
	for (UniformMap::iterator itor = m_uniformMap.begin(); itor != m_uniformMap.end(); ++itor)
	{
		desc = itor->second;
		delete desc;
	}

	m_uniformMap.clear();
	m_uniformVec.clear();

	if (m_handle)
	{
		DeleteProgram(m_handle);
		m_handle = 0;
	}
}

void OGL_Program::attach(ShaderStage* stage)
{
	OGL_Shader* shader = (OGL_Shader*)stage;
	ShaderType type = shader->getShaderType();
#ifdef CU_USE_SPO
	glUseProgramStages(m_handle, mapping_bit(type), shader->native());
#else
	glAttachShader(m_handle, shader->native());
#endif
	m_shaders[type] = shader;
}

void OGL_Program::link()
{
	if (!m_shaders[SHADER_VERTEX])
		return;

#ifndef CU_USE_SPO
	GLint result;
	glLinkProgram(m_handle);
	glGetProgramiv(m_handle, GL_LINK_STATUS, &result);
	bool success = (result == GL_TRUE);
	if (!success)
	{
		return;
	}
#endif

	// parse attribute
#ifdef CU_USE_SPO
	parseAttribute(m_shaders[SHADER_VERTEX]->native());
#else
	parseAttribute(m_handle);
#endif

	// parse uniforms
#ifdef CU_USE_SPO
	for (int i = 0; i < SHADER_COUNT; ++i)
	{
		if (!m_shaders[i])
			continue;
		GLuint handle = m_shaders[i]->native();
		// 解析
		UniformVec uniforms;
		bool result = parseUniform(handle, uniforms);
		if (!result)
			continue;

	}
#else
	UniformVec uniforms;
	parseUniform(m_handle, uniforms);
	m_uniformVec.reserve(uniforms.size());
	UniformDesc* desc;
	// 建立map
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		desc = uniforms[i];

		UniformMap::iterator itor = m_uniformMap.find(desc->name);
		if (itor != m_uniformMap.end())
		{
			// 说明已经存在,应该校验类型必须一致,目前直接忽略
			delete desc;
			continue;
		}

		// 需要重新建立索引??
		m_uniformMap[desc->name] = desc;
		if (!desc->isVariable())
			m_uniformVec.push_back(*desc);
	}
#endif
}

void OGL_Program::bind()
{
	UseProgram(m_handle);
}

GLint OGL_Program::getLocation(Semantic semantic)
{
	return m_attrs[semantic];
}

bool OGL_Program::parseAttribute(GLuint handle)
{
	memset(m_attrs, 0, sizeof(m_attrs));

	bool successed = true;
	// 解析attr
	GLint attr_num, len;
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &attr_num);
	if (attr_num > 0)
		glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &len);
	if (attr_num > 0 && len > 0)
	{
		GLchar* name = new GLchar[len + 1];
		GLint size;
		GLenum type;
		GLint location;
		for (int i = 0; i < attr_num; ++i)
		{
			glGetActiveAttrib(handle, i, len, NULL, &size, &type, name);
			name[len] = '\0';
			location = glGetAttribLocation(handle, name);
			// 设置
			int index = find_usage(name);
			// 映射，将名字映射成Position
			if (index != -1)
				m_attrs[index] = location;
			else
				successed = false;
		}
	}
	return successed;
}

bool OGL_Program::parseUniform(GLuint handle, UniformVec& uniforms)
{
	// get max buffer size
	GLint bufSize;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
#ifdef CU_USE_UNIFORM_BLOCK
	GLint blockNameSize;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &blockNameSize);
	if (blockNameSize > bufSize)
		bufSize = blockNameSize;
#endif

	// create buffer
	UniformDesc* desc = NULL;
	GLchar* name = new GLchar[bufSize];

	// parse block
#ifdef CU_USE_UNIFORM_BLOCK
	GLint blockCount = 0;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount);
	for (GLint i = 0; i < blockCount; ++i)
	{
		GLsizei blockSize = 0;
		glGetActiveUniformBlockName(handle, i, bufSize, &blockSize, name);

		desc = new UniformDesc();
		desc->name = name;
		desc->type = UT_CBUFFER;
		desc->slot = i;
		desc->bytes = blockSize;
		uniforms.push_back(desc);
	}
#endif

	// parse active uniforms
	GLenum	type;
	GLint	size;
	GLint	activeCount;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &activeCount);
	for (GLuint index = 0; index < (GLuint)activeCount; ++index)
	{
		glGetActiveUniform(handle, index, bufSize, NULL, &size, &type, name);

		// If this is an array uniform, strip array indexers off it since GL does not
		// seem to be consistent across different drivers/implementations in how it returns
		// array uniforms. On some systems it will return "u_matrixArray", while on others
		// it will return "u_matrixArray[0]".
		char* c = strchr(name, '[');
		if (c)
			*c = '\0';

		desc = new UniformDesc();
		desc->name = name;
		desc->type = OGL_Mapping::getUniformType(type);
		desc->bytes = size;	// calc

		// check is variable
#ifdef CU_USE_UNIFORM_BLOCK
		GLint blockIndex = -1;
		glGetActiveUniformsiv(handle, 1, &index, GL_UNIFORM_BLOCK_INDEX, &blockIndex);
		if (blockIndex != -1)
		{
			GLint offset;
			glGetActiveUniformsiv(handle, 1, &index, GL_UNIFORM_OFFSET, &offset);
			desc->index = index;
			desc->offset = offset;
			uniforms.push_back(desc);
			continue;
		}
#endif

		desc->slot = glGetUniformLocation(handle, name);
		uniforms.push_back(desc);
	}

	return true;
}

CU_NS_END