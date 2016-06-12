#pragma once
#include "OGL_Shader.h"

CU_NS_BEGIN

class CU_OGL_API OGL_Program : public ShaderProgram
{
public:
	OGL_Program();
	~OGL_Program();

	void attach(ShaderStage* stage);
	void link();

	void bind();

private:
	bool parseAttribute(GLuint handle);
	bool parseUniform(GLuint handle);

private:
	typedef std::map<uint8_t, OGL_Shader*> ShaderMap;
	GLuint		m_handle;
	ShaderMap	m_shaders;
	// ½âÎö
	GLint		m_attrs[SEMANTIC_MAX];
};

CU_NS_END