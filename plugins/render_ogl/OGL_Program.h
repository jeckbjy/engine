#pragma once
#include "OGL_Shader.h"

CU_NS_BEGIN

// glProgramUniform glUniform
class CU_OGL_API OGL_Program : public ShaderProgram
{
public:
	OGL_Program(uint32_t id);
	~OGL_Program();

	void attach(ShaderStage* stage);
	void link();

	void bind();

	GLint getLocation(Semantic semantic);

	const UniformVec& getUniformVec() const { return m_uniformVec; }

private:
	bool parseAttribute(GLuint handle);
	bool parseUniform(GLuint handle, UniformVec& uniforms);

private:
	typedef SharedPtr<OGL_Shader> ShaderPtr;
	GLuint		m_handle;
	ShaderPtr	m_shaders[SHADER_COUNT];
	// ����
	GLint		m_attrs[SEMANTIC_MAX];
	UniformVec	m_uniformVec;	// ������descriptor
	UniformMap	m_uniformMap;	// ����Variable��descriptor
};

CU_NS_END