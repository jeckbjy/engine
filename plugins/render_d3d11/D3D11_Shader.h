#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_Shader : public ShaderStage
{
public:
	D3D11_Shader(uint32_t id);
	~D3D11_Shader();

	bool compile(const ShaderDesc& desc);

	ID3DBlob* getCode() { return m_code; }

	ID3D11DeviceChild* getHandle() { return m_shader; }

	UniformVec& getUniforms() { return m_uniforms; }

private:
	bool create(ShaderType stage, DWORD* code, SIZE_T size);
	void parse();

	UniformType getInputType(D3D_SHADER_INPUT_TYPE type, D3D_SRV_DIMENSION dimension);
	UniformType getVariableType(D3D11_SHADER_TYPE_DESC& desc);

private:
	ID3D11DeviceChild*	m_shader;
	ID3DBlob*			m_code;		// vsÐèÒª
	UniformVec			m_uniforms;
};

CU_NS_END