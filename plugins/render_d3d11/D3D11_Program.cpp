#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11_Program::D3D11_Program()
{
	for (size_t i = 0; i < SHADER_COUNT; ++i)
	{
		m_shaders[i] = NULL;
	}
}

D3D11_Program::~D3D11_Program()
{
	for (size_t i = 0; i < SHADER_COUNT; ++i)
	{
		CU_SAFE_RELEASE(m_shaders[i]);
	}
}

void D3D11_Program::attach(ShaderStage* shader)
{
	ShaderType type = shader->getShaderType();
	if (type < SHADER_COUNT)
	{
		CU_SAFE_RELEASE(m_shaders[type]);
		m_shaders[type] = (D3D11_Shader*)shader;
	}
}

void D3D11_Program::link()
{
	//// 要求同名必须有相同结构
	//typedef std::map<String, size_t> UniformIndexMap;
	//UniformIndexMap indexMap;
	//// 合并参数
	//for (ProgramMap::iterator itor = m_shaders.begin(); itor != m_shaders.end(); ++itor)
	//{
	//	D3D11Program* shader = itor->second;
	//	UniformMap& inputs = shader->getInput();
	//	for (UniformMap::iterator uniformItor = inputs.begin(); uniformItor != inputs.end(); ++uniformItor)
	//	{
	//		UniformDesc& desc = uniformItor->second;
	//		UniformIndexMap::iterator indexItor = indexMap.find(desc.name);
	//		if (indexItor != indexMap.end())
	//		{
	//			// 校验是否一致
	//			DescriptorInfo& info = m_descriptorVec[indexItor->second];
	//			UniformDesc& dstDesc = info.desc;
	//			if (dstDesc.type != desc.type)
	//				continue;
	//			info.slots[shader->getShaderType()] = desc.slot;
	//		}
	//		else
	//		{
	//			DescriptorInfo info;
	//			info.desc = desc;
	//			info.slots[shader->getShaderType()] = desc.slot;
	//		}
	//	}

	//	UniformMap& variable = shader->getVariable();

	//}
}

void D3D11_Program::bind(ID3D11ContextN* context)
{
	ID3D11DeviceChild* shader;

	shader = m_shaders[SHADER_VERTEX] ? m_shaders[SHADER_VERTEX]->getHandle() : NULL;
	context->VSSetShader((ID3D11VertexShader*)shader, NULL, 0);

	shader = m_shaders[SHADER_PIXEL] ? m_shaders[SHADER_PIXEL]->getHandle() : NULL;
	context->PSSetShader((ID3D11PixelShader*)shader, NULL, 0);

	shader = m_shaders[SHADER_GEOMETRY] ? m_shaders[SHADER_GEOMETRY]->getHandle() : NULL;
	context->GSSetShader((ID3D11GeometryShader*)shader, NULL, 0);

	shader = m_shaders[SHADER_HULL] ? m_shaders[SHADER_HULL]->getHandle() : NULL;
	context->HSSetShader((ID3D11HullShader*)shader, NULL, 0);

	shader = m_shaders[SHADER_DOMAIN] ? m_shaders[SHADER_DOMAIN]->getHandle() : NULL;
	context->DSSetShader((ID3D11DomainShader*)shader, NULL, 0);

	shader = m_shaders[SHADER_COMPUTE] ? m_shaders[SHADER_COMPUTE]->getHandle() : NULL;
	context->CSSetShader((ID3D11ComputeShader*)shader, NULL, 0);
}

CU_NS_END