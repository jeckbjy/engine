#pragma once
#include "D3D11_DescriptorSet.h"
#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11_DescriptorSet::D3D11_DescriptorSet(D3D11_Program* program)
	: m_program(program)
{
	UniformVec& uniforms = m_program->getUniformVec();
	if (!uniforms.empty())
	{
		m_descriptor.resize(uniforms.size());
		for (size_t i = 0; i < uniforms.size(); ++i)
		{
			UniformDesc* desc = uniforms[i];

			//m_descriptor[i] = new void*[desc->arrays];
			//if (desc->type == UT_CBUFFER)
			//{
			//	for (size_t j = 0; j < desc->arrays; ++i)
			//		m_descriptor[i][j] = new D3D11_Buffer();
			//}
		}
	}
}

D3D11_DescriptorSet::~D3D11_DescriptorSet()
{

}

void D3D11_DescriptorSet::setValue(const String& name, Texture* texture, size_t index)
{
	//UniformVec* uniforms = m_program->getUniformVecByName(name);
	//if (uniforms == NULL)
	//	return;
	//UniformDesc* desc = uniforms->at(0);
	//if (desc->type < UT_TEXTURE1D || desc->type > UT_TEXTURE2DMS)
	//if (index >= desc->arrays)
	//	return;
	//m_descriptor[desc->index][index] = texture;
}

void D3D11_DescriptorSet::setValue(const String& name, void* data, size_t size, size_t offset)
{
	//UniformVec* uniforms = m_program->getUniformVecByName(name);
	//if (uniforms == NULL)
	//	return;
	//UniformDesc* desc = uniforms->at(0);
	//if (!desc->isVariable())
	//	return;
	//UniformDesc* owner;
	//// 每一个都要设置
	//for (size_t i = 0; i < uniforms->size(); ++i)
	//{
	//	desc = uniforms->at(i);
	//	if (desc->index >= m_descriptor.size())
	//		continue;
	//	owner = m_descriptor[desc->index];
	//	if (owner->type != UT_CBUFFER)
	//		continue;
	//	D3D11_Buffer* buffer = m_descriptor[owner->index];
	//}
}

void D3D11_DescriptorSet::bind(ID3D11ContextN* context)
{
	// 描述信息
	//D3D11ProgramPipeline::DescriptorVec descriptor = m_pipeline->getDescriptors();
	//for (size_t i = 0; i < m_descriptor.size(); ++i)
	//{
	//	// 每一个去绑定
	//}
}

CU_NS_END