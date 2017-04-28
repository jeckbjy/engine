#include "D3D12_DescriptorSet.h"

CUTE_NS_BEGIN

D3D12_DescriptorSet::D3D12_DescriptorSet()
{

}

D3D12_DescriptorSet::~D3D12_DescriptorSet()
{
	D3D12_RELEASE(m_descriptors);
}

CUTE_NS_END
