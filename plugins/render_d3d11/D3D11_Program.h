#pragma once
#include "D3D11_Shader.h"

CU_NS_BEGIN

/*
解析Desc：一个Material可以含有多个Pass，每个Pass含有多个Stage，每个Stage都是独立的，
故此：同一个UniformBlock中的成员可以含有相同的名字，但是属于不同Block
限制要求：
相同名字的成员一定拥有相同的类型
但是相同的Variable可以分别属于同的Stage中的Block，则需要设计成一个列表
潜规则：不同Shader中会有一个同名的Global字段
*/
class D3D11_Shader;
class CU_D3D11_API D3D11_Program : public ShaderProgram
{
public:
	typedef std::map<uint8_t, uint32_t> SlotMap;
	// 每个描述信息
	struct DescriptorInfo
	{
		UniformDesc desc;
		SlotMap		slots;
	};

	typedef std::vector<DescriptorInfo>			DescriptorVec;
	typedef std::map<String, UniformDesc>		DescriptorMap;
	typedef std::map<uint8_t, D3D11_Shader*>	ProgramMap;

public:
	D3D11_Program();
	~D3D11_Program();

	void attach(ShaderStage* shader);
	void link();
	void bind(ID3D11ContextN* context);

	D3D11_Shader* getVS() { return m_shaders[SHADER_VERTEX]; }

	DescriptorVec& getDescriptors() { return m_descriptorVec; }

private:
	D3D11_Shader*	m_shaders[SHADER_COUNT];
	DescriptorVec	m_descriptorVec;
	DescriptorMap	m_descriptorMap;
};

CU_NS_END