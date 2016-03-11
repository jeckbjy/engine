#include "D3D12_DescriptorSet.h"

CU_NS_BEGIN

D3D12DescriptorSet::D3D12DescriptorSet()
{

}

D3D12DescriptorSet::~D3D12DescriptorSet()
{
	D3D12_RELEASE(m_descriptors);
}

CU_NS_END
