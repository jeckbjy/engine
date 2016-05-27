#pragma once
#include "D3D11_Shader.h"

CU_NS_BEGIN

/*
����Desc��һ��Material���Ժ��ж��Pass��ÿ��Pass���ж��Stage��ÿ��Stage���Ƕ����ģ�
�ʴˣ�ͬһ��UniformBlock�еĳ�Ա���Ժ�����ͬ�����֣��������ڲ�ͬBlock
����Ҫ��
��ͬ���ֵĳ�Աһ��ӵ����ͬ������
������ͬ��Variable���Էֱ�����ͬ��Stage�е�Block������Ҫ��Ƴ�һ���б�
Ǳ���򣺲�ͬShader�л���һ��ͬ����Global�ֶ�
*/
class D3D11_Shader;
class CU_D3D11_API D3D11_Program : public ShaderProgram
{
public:
	typedef std::map<uint8_t, uint32_t> SlotMap;
	// ÿ��������Ϣ
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