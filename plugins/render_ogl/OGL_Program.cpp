#include "OGL_Program.h"
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

OGL_Program::OGL_Program()
:m_handle(0)
{
	CreateProgram(m_handle);
	for (int i = 0; i < SEMANTIC_MAX; ++i)
	{
		m_attrs[i] = -1;
	}
}

OGL_Program::~OGL_Program()
{
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
	bool successed = true;
#ifdef CU_USE_SPO
	//ShaderMap::iterator itor = m_shaders.find(ST_VERTEX);
	//if (itor != m_shaders.end() && !parse_attrs(itor->second))
	//	successed = true;
	//else
	//	successed = false;
	//todo: 对所有的shader解析uniform
#else
	GLint success;
	glLinkProgram(m_handle);
	glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
	successed = (success == GL_TRUE);
	//if (!parse_attrs(m_handle))
	//	successed = false;
	//if (!parse_uniform(m_handle))
	//	successed = false;
#endif
	//return successed;
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

bool OGL_Program::parseUniform(GLuint handle)
{
	return true;
}

CU_NS_END