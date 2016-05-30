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
	IndexMap indexs;

	D3D11_Shader* shader;
	UniformDesc* srcDesc;
	UniformDesc* dstDesc;
	UniformDesc* tmpDesc;
	for (uint8_t i = 0; i < SHADER_COUNT; ++i)
	{
		shader = m_shaders[i];
		if (!shader)
			continue;
		UniformVec& uniforms = shader->getUniforms();
		for (size_t j = 0; j < uniforms.size(); ++j)
		{
			srcDesc = uniforms[j];
			UniformMap::iterator itor = m_uniformMap.find(srcDesc->name);
			// 相同名字的必须有相同的类型和数组个数
			if (itor != m_uniformMap.end())
			{
				tmpDesc = itor->second[0];
				if (tmpDesc->type != srcDesc->type || tmpDesc->arrays != tmpDesc->arrays)
					continue;
			}

			dstDesc = new UniformDesc();
			*dstDesc = *srcDesc;
			// 设置索引
			if (srcDesc->isVariable())
			{
				// find owner, must exist!
				tmpDesc = uniforms[srcDesc->index];
				dstDesc->index = indexs[tmpDesc->name];
			}
			else
			{
				dstDesc->index = indexs[srcDesc->name];
			}

			// 插入
			if (itor == m_uniformMap.end())
			{
				// 不存在，新建
				if (!srcDesc->isVariable())
				{
					m_uniformVec.push_back(dstDesc);
					indexs[dstDesc->name] = m_uniformVec.size() - 1;
				}
			}
			m_uniformMap[srcDesc->name].push_back(dstDesc);
		}
	}
}

UniformVec* D3D11_Program::getUniformVecByName(const String& name)
{
	UniformMap::iterator itor = m_uniformMap.find(name);
	if (itor == m_uniformMap.end())
		return NULL;
	return &(itor->second);
}

CU_NS_END