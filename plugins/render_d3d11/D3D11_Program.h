#pragma once
#include "D3D11_Shader.h"

CU_NS_BEGIN

class D3D11_Shader;
typedef std::map<String, UniformVec> UniformMap;
class CU_D3D11_API D3D11_Program : public ShaderProgram
{
public:
	D3D11_Program();
	~D3D11_Program();

	void attach(ShaderStage* shader);
	void link();
	void bind(ID3D11ContextN* context);

	D3D11_Shader* getVS() { return m_shaders[SHADER_VERTEX]; }

	UniformVec& getUniformVec() { return m_uniformVec; }
	UniformVec* getUniformVecByName(const String& name);

private:
	D3D11_Shader*	m_shaders[SHADER_COUNT];
	UniformVec		m_uniformVec;
	UniformMap		m_uniformMap;
};

CU_NS_END