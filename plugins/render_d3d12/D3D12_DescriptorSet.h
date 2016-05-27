#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_DescriptorSet : public DescriptorSet
{
public:
	D3D12_DescriptorSet();
	~D3D12_DescriptorSet();

private:
	ID3D12DescriptorHeap* m_descriptors;
};

CU_NS_END