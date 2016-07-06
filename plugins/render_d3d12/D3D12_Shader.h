#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_Shader : public ShaderStage
{
public:
	D3D12_Shader(uint32_t id);
	~D3D12_Shader();

	bool compile(const ShaderDesc& desc);

	ID3DBlob* getCode() { return m_code; }

private:
	void parse();

private:
	ID3DBlob* m_code;
};

CU_NS_END