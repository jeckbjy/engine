#pragma once
#include "D3D11_DescriptorSet.h"
#include "D3D11_Pipeline.h"
#include "D3D11_Program.h"

CU_NS_BEGIN

D3D11_DescriptorSet::D3D11_DescriptorSet(D3D11_Program* pipeline)
	: m_pipeline(pipeline)
{

}

D3D11_DescriptorSet::~D3D11_DescriptorSet()
{

}

void D3D11_DescriptorSet::setValue(const String& name, Texture* texture, size_t index)
{
	// ���Ȳ���DecriptorDesc
}

void D3D11_DescriptorSet::setValue(const String& name, void* data, size_t size, size_t offset)
{

}

void D3D11_DescriptorSet::bind(ID3D11ContextN* context)
{
	// ������Ϣ
	//D3D11ProgramPipeline::DescriptorVec descriptor = m_pipeline->getDescriptors();
	//for (size_t i = 0; i < m_descriptor.size(); ++i)
	//{
	//	// ÿһ��ȥ��
	//}
}

CU_NS_END