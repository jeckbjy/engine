#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_DescriptorSetLayout : public IDescriptorSetLayout
{
public:
	D3D12_DescriptorSetLayout();
	~D3D12_DescriptorSetLayout();

	bool init(ID3D12Device* device, const DescriptorSetLayoutDesc& desc);
	void term();

	ID3D12RootSignature* getRootSignature() const;

private:
	ID3D12RootSignature*	m_signature;
	bool					m_isGraphics;
};

CUTE_NS_END
