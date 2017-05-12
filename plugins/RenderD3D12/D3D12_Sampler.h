#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class D3D12_Device;
class D3D12_Descriptor;
class CUTE_D3D12_API D3D12_Sampler : public ISampler
{
public:
	D3D12_Sampler();
	~D3D12_Sampler();

	bool init(D3D12_Device* device, const SamplerDesc& desc);
	void term();

	D3D12_GPU_DESCRIPTOR_HANDLE getHandleGPU() const;

private:
	D3D12_Device*		m_device;
	D3D12_Descriptor*	m_descriptor;
};

CUTE_NS_END
