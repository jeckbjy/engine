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
	int		block;	// ���ڵ�block
	int		offset;
	int		size;
};

struct Uniform
{
	String	name;
	uint8_t category;	// UniformType
	GLint	type;		// ��������
	GLint	location;;	// ��λ��
	GLint	index;		// ͼƬ����
	GLint	arrays;		// �����С
	GLint	size;		// �ڴ��ܴ�С
};

struct ParamInfo
{
	uint8_t type;
	size_t index;	// ����block
	size_t offset;
	size_t size;	// �ܴ�С
	ParamInfo(){ memset(this, 0, sizeof(*this)); }
	ParamInfo(uint8_t type, size_t index, size_t offset, size_t size) 
		:type(type), index(index), offset(offset), size(size)
	{}
};

// ��Ϊ����ģʽ��SPO�ͷ�SPO
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
	typedef std::map<String, ParamInfo>	ParamMap;	// ����
	typedef std::map<int, GLuint>		ShaderMap;

	GLuint		m_handle;
	GLint		m_attrs[VERTEX_USAGE_MAX];	// ���ж�ӦAttr
	ShaderMap	m_shaders;
	UniformVec	m_uniforms;		// ����Ϣ
	VariableVec	m_variables;	// Uniform����
	ParamMap	m_params;		// ��������
};

CU_NS_END