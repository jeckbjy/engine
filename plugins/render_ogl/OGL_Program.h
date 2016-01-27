#pragma once
#include "OGL_API.h"
#include <map>

CU_NS_BEGIN

enum UniformType
{
	UT_NONE,
	UT_BLOCK,
	UT_BUFFER,
	UT_TEXTURE,
};

enum
{
	PT_VARIABLE = 1,
	PT_UNIFORM,
};

struct Variable
{
	String	name;
	int		block;	// 所在的block
	int		offset;
	int		size;
};

struct Uniform
{
	String	name;
	uint8_t category;	// UniformType
	GLint	type;		// 数据类型
	GLint	location;;	// 绑定位置
	GLint	index;		// 图片索引
	GLint	arrays;		// 数组大小
	GLint	size;		// 内存总大小
};

struct ParamInfo
{
	uint8_t type;
	size_t index;	// 所在block
	size_t offset;
	size_t size;	// 总大小
	ParamInfo(){ memset(this, 0, sizeof(*this)); }
	ParamInfo(uint8_t type, size_t index, size_t offset, size_t size) 
		:type(type), index(index), offset(offset), size(size)
	{}
};

// 分为两种模式，SPO和非SPO
class CU_OGL_API OGLProgram : public Program
{
public:
	OGLProgram();
	~OGLProgram();

	bool attach(ShaderType type, const String& data, const String& name /* = "" */, const String& entry /* = "" */, ShaderProfile profile /* = SP_NONE */, bool binary /* = false */);
	bool link();
	void bind(DescriptorSet* descriptors);
	void unbind();

	GLuint compile(ShaderType type, const String& data);
	GLuint load(ShaderType type, const String& data);

	bool parse_attrs(GLuint handle);
	bool parse_uniform(GLuint handle);

	const ParamInfo* getInfo(const String& name) const;

	inline GLuint native() const { return m_handle; }
	inline GLint getAttribute(VertexUsage usage) { return m_attrs[usage]; }
	inline size_t uniformCount() const { return m_uniforms.size(); }
	inline Uniform* getUniform(size_t index) { return &m_uniforms[index]; }

private:
	typedef std::vector<Variable>		VariableVec;
	typedef std::vector<Uniform>		UniformVec;
	typedef std::map<String, ParamInfo>	ParamMap;	// 查找
	typedef std::map<int, GLuint>		ShaderMap;

	GLuint		m_handle;
	GLint		m_attrs[VERTEX_USAGE_MAX];	// 所有对应Attr
	ShaderMap	m_shaders;
	UniformVec	m_uniforms;		// 绑定信息
	VariableVec	m_variables;	// Uniform变量
	ParamMap	m_params;		// 用于索引
};

CU_NS_END