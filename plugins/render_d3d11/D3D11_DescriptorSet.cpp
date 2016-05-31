#pragma once
#include "D3D11_DescriptorSet.h"
#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"
#include "D3D11_Device.h"
#include "D3D11_Buffer.h"
#include "D3D11_Texture.h"

CU_NS_BEGIN

D3D11_DescriptorSet::D3D11_DescriptorSet(D3D11_Program* program, D3D11_Device* device)
	: m_program(program)
{
	UniformVec& uniforms = m_program->getUniformVec();
	if (!uniforms.empty())
	{
		m_descriptors.resize(uniforms.size());
		for (size_t i = 0; i < uniforms.size(); ++i)
		{
			UniformDesc* desc = uniforms[i];
			switch (desc->type)
			{
			case UT_CBUFFER:
			{
				BufferDesc buffDesc(BU_UNIFORM, desc->bytes, 1, RES_DEFAULT, NULL);
				D3D11_Buffer* buffer = (D3D11_Buffer*)device->newBuffer(buffDesc);
				m_descriptors[i].set(TYPE_BUFFER, buffer);
				break;
			}
			case UT_TEXTURE1D:
			case UT_TEXTURE2D:
			case UT_TEXTURE3D:
			case UT_TEXTURECUBE:
			case UT_TEXTURE2DMS:
			{
				D3D11_Texture** textures = new D3D11_Texture*[desc->arrays];
				for (uint32_t j = 0; j < desc->arrays; ++j)
					textures[j] = NULL;

				m_descriptors[i].set(TYPE_TEXTURE, textures);
				break;
			}
			default:
				break;
			}
		}
	}
}

D3D11_DescriptorSet::~D3D11_DescriptorSet()
{

}

void D3D11_DescriptorSet::setValue(const String& name, Texture* texture, size_t index)
{
	UniformDesc* desc = m_program->getUniformByName(name);
	if (desc == NULL || desc->index >= m_descriptors.size() || index >= desc->arrays)
		return;
	Descriptor& descriptor = m_descriptors[desc->index];
	if (descriptor.type != TYPE_TEXTURE)
		return;
	descriptor.textures[index] = (D3D11_Texture*)texture;
}

void D3D11_DescriptorSet::setValue(const String& name, void* data, size_t size, size_t offset)
{
	UniformDesc* desc = m_program->getUniformByName(name);
	if (!desc || !desc->isVariable())
		return;

	UniformDesc* owner;
	uint64_t varInfo;
	uint32_t varIdx;
	uint32_t varOff;
	Descriptor* descriptor;
	for (StageInfoMap::iterator itor = desc->stages.begin(); itor != desc->stages.end(); ++itor)
	{
		varInfo = itor->second;
		varIdx = varInfo >> 32;
		varOff = varInfo & 0xFFFFFFFF;
		if (varIdx >= m_descriptors.size())
			continue;
		owner = m_program->getUniformByIndex(varIdx);
		descriptor = &(m_descriptors[owner->index]);
		if (descriptor->type != TYPE_BUFFER)
			continue;
		D3D11_Buffer* buffer = descriptor->buffer;
		// 拷贝数据
	}
}

void D3D11_DescriptorSet::bind(ID3D11ContextN* context)
{
	if (m_descriptors.empty())
		return;

	uint32_t slot;
	ID3D11ShaderResourceView* views[32];
	size_t count = m_descriptors.size();
	for (size_t i = 0; i < m_descriptors.size(); ++i)
	{
		Descriptor& descriptor = m_descriptors[i];
		UniformDesc* desc = m_program->getUniformByIndex(i);
		switch (descriptor.type)
		{
		case TYPE_BUFFER:
		{
			ID3D11Buffer* buffers[1];
			buffers[0] = descriptor.buffer->native();
			// 要求有多个
			for (StageInfoMap::iterator itor = desc->stages.begin(); itor != desc->stages.end(); ++itor)
			{
				slot = itor->second;
				switch (itor->first)
				{
				case SHADER_VERTEX:
					context->VSSetConstantBuffers(slot, 1, buffers);
					break;
				case SHADER_PIXEL:
					context->PSSetConstantBuffers(slot, 1, buffers);
					break;
				case SHADER_GEOMETRY:
					context->GSSetConstantBuffers(slot, 1, buffers);
					break;
				case SHADER_HULL:
					context->HSSetConstantBuffers(slot, 1, buffers);
					break;
				case SHADER_DOMAIN:
					context->DSSetConstantBuffers(slot, 1, buffers);
					break;
				case SHADER_COMPUTE:
					context->CSSetConstantBuffers(slot, 1, buffers);
					break;
				}
			}
			break;
		}
		case TYPE_TEXTURE:
		{
			for (size_t i = 0; i < desc->arrays; ++i)
			{
				D3D11_Texture* texture = descriptor.textures[i];
				views[i] = texture ? texture->getSRV() : NULL;
			}
			size_t count = desc->arrays;
			// 绑定
			for (StageInfoMap::iterator itor = desc->stages.begin(); itor != desc->stages.end(); ++itor)
			{
				slot = itor->second;
				switch (itor->first)
				{
				case SHADER_VERTEX:
					context->VSSetShaderResources(slot, count, views);
					break;
				case SHADER_PIXEL:
					context->PSSetShaderResources(slot, count, views);
					break;
				case SHADER_GEOMETRY:
					context->GSSetShaderResources(slot, count, views);
					break;
				case SHADER_HULL:
					context->HSSetShaderResources(slot, count, views);
					break;
				case SHADER_DOMAIN:
					context->DSSetShaderResources(slot, count, views);
					break;
				case SHADER_COMPUTE:
					context->CSSetShaderResources(slot, count, views);
					break;
				}
			}
			break;
		}
		}
	}
}

CU_NS_END