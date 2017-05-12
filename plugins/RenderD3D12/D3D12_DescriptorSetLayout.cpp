#include "D3D12_DescriptorSetLayout.h"
#include "D3D12_Mapping.h"

CUTE_NS_BEGIN

D3D12_DescriptorSetLayout::D3D12_DescriptorSetLayout()
	: m_signature(NULL)
{
}

D3D12_DescriptorSetLayout::~D3D12_DescriptorSetLayout()
{
	term();
}

bool D3D12_DescriptorSetLayout::init(ID3D12Device* device, const DescriptorSetLayoutDesc& desc)
{
	return true;
}

void D3D12_DescriptorSetLayout::term()
{
}

ID3D12RootSignature* D3D12_DescriptorSetLayout::getRootSignature() const
{
	return m_signature;
}

CUTE_NS_END
