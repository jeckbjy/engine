#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_Shader : public ShaderStage
{
public:
	D3D11_Shader(uint32_t id);
	~D3D11_Shader();

	bool compile(const ProgramDesc& desc);

	uint32_t getID() const { return m_id; }
	ID3DBlob* getCode() { return m_code; }

	UniformMap& getInput()  { return m_inputs; }
	UniformMap& getVariable() { return m_variables; }

	ID3D11DeviceChild* getHandle() { return m_shader; }

private:
	bool create(ShaderType stage, DWORD* code, SIZE_T size);
	void parse();
	void parseVariable(ID3D11ShaderReflectionVariable*	variable);

	UniformType getInputType(D3D_SHADER_INPUT_TYPE type, D3D_SRV_DIMENSION dimension);
	UniformType getVariableType(D3D11_SHADER_TYPE_DESC& desc);

private:
	ShaderType			m_type;
	ID3D11DeviceChild*	m_shader;
	ID3DBlob*			m_code;		// vs需要
	uint32_t			m_id;		// 唯一ID
	// 解析
	UniformMap			m_inputs;
	UniformMap			m_variables;
};

CU_NS_END