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
	if (!shader)
		return;
	ShaderType type = shader->getShaderType();
	if (type < SHADER_COUNT)
	{
		CU_SAFE_RELEASE(m_shaders[type]);
		m_shaders[type] = (D3D11_Shader*)shader;
	}
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

void D3D11_Program::link()
{
	typedef std::map<String, size_t> IndexMap;
	IndexMap indexMap;
	uint64_t stageInfo;
	uint64_t index;

	D3D11_Shader* shader;
	UniformDesc* srcDesc;
	UniformDesc* dstDesc;
	for (uint8_t i = 0; i < SHADER_COUNT; ++i)
	{
		shader = m_shaders[i];
		if (!shader)
			continue;
		UniformVec& uniforms = shader->getUniforms();
		for (size_t j = 0; j < uniforms.size(); ++j)
		{
			srcDesc = uniforms[j];
			// 先求出索引
			if(srcDesc->isVariable())
			{
				dstDesc = uniforms[srcDesc->index];
				index = indexMap[dstDesc->name];
				stageInfo = (index << 32) + srcDesc->offset;
			}
			else
			{
				index = indexMap[srcDesc->name];
				stageInfo = srcDesc->slot;
			}

			UniformMap::iterator itor = m_uniformMap.find(srcDesc->name);
			// 相同名字的必须有相同的类型和数组个数
			if (itor != m_uniformMap.end())
			{
				dstDesc = itor->second;
				if (dstDesc->type != srcDesc->type ||
					dstDesc->arrays != srcDesc->arrays ||
					dstDesc->bytes != srcDesc->bytes)
					continue;

				dstDesc->stages[i] = stageInfo;
			}
			else
			{
				dstDesc = new UniformDesc();
				*dstDesc = *srcDesc;
				dstDesc->stages[i] = stageInfo;

				m_uniformMap[dstDesc->name] = dstDesc;
				// 创建Descriptor
				if (!srcDesc->isVariable())
				{
					m_uniformVec.push_back(dstDesc);
					indexMap[dstDesc->name] = m_uniformVec.size() - 1;
				}
			}
		}
	}
}

UniformDesc* D3D11_Program::getUniformByName(const String& name)
{
	UniformMap::iterator itor = m_uniformMap.find(name);
	if (itor == m_uniformMap.end())
		return NULL;
	return itor->second;
}

CU_NS_END