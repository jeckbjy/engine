#include "D3D12_DescriptorSet.h"

CUTE_NS_BEGIN

D3D12_DescriptorSet::D3D12_DescriptorSet()
{
}

D3D12_DescriptorSet::~D3D12_DescriptorSet()
{
	term();
}

bool D3D12_DescriptorSet::init(ID3D12Device* device, size_t entryCount, bool isGraphics)
{
	m_handles.resize(entryCount);
	m_isGraphics = isGraphics;
	return true;
}

void D3D12_DescriptorSet::term()
{
	m_handles.clear();
}

void D3D12_DescriptorSet::setBuffer(size_t index, IBuffer* buffer)
{
}

void D3D12_DescriptorSet::setTexture(size_t index, ITextureView* texture)
{

}

void D3D12_DescriptorSet::setSampler(size_t index, ISampler* sampler)
{
}

CUTE_NS_END
