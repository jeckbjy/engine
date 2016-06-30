#pragma once
#include "D3D11_Shader.h"

CU_NS_BEGIN

class D3D11_Shader;
typedef std::map<String, UniformDesc*> UniformMap;
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
	UniformDesc* getUniformByName(const String& name);
	UniformDesc* getUniformByIndex(size_t index) { return m_uniformVec[index]; }

private:
	D3D11_Shader*	m_shaders[SHADER_COUNT];
	UniformVec		m_uniformVec;	// 仅含有descriptor
	UniformMap		m_uniformMap;	// 含有Variable和descriptor
};

CU_NS_END