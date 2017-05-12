#include "D3D12_Sampler.h"
#include "D3D12_Device.h"
#include "D3D12_Sampler.h"
#include "D3D12_Mapping.h"
#include "D3D12_Descriptor.h"
#include "D3D12_DescriptorHeap.h"

CUTE_NS_BEGIN

D3D12_Sampler::D3D12_Sampler()
	: m_device(NULL)
	, m_descriptor(NULL)
{
}

D3D12_Sampler::~D3D12_Sampler()
{
	term();
}

bool D3D12_Sampler::init(D3D12_Device* device, const SamplerDesc& desc)
{
	m_device = device;
	D3D12_SAMPLER_DESC info = {};
	info.Filter			= D3D12_Mapping::getFilter(desc.minFilter, desc.magFilter, desc.mipmapMode, desc.compareEnable, desc.anisotropyEnable);
	info.AddressU		= D3D12_Mapping::getAddressMode(desc.addressU);
	info.AddressV		= D3D12_Mapping::getAddressMode(desc.addressV);
	info.AddressW		= D3D12_Mapping::getAddressMode(desc.addressW);
	info.MipLODBias		= desc.mipLodBias;
	info.ComparisonFunc	= D3D12_Mapping::getCompareOp(desc.compareOp);
	info.MinLOD			= desc.minLod;
	info.MaxLOD			= desc.maxLod;
	D3D12_Mapping::getBorderColor(desc.borderColor, info.BorderColor);

	D3D12_DescriptorHeap* heap = device->getDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	m_descriptor = heap->create();

	ID3D12Device* native = device->getDevice();
	native->CreateSampler(&info, m_descriptor->getHandleCPU());

	return true;
}

void D3D12_Sampler::term()
{
	if (m_device == NULL || m_descriptor == NULL)
		return;

	D3D12_DescriptorHeap* heap = m_device->getDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	heap->free(m_descriptor);

	m_device = NULL;
	m_descriptor = NULL;
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12_Sampler::getHandleGPU() const
{
	return m_descriptor->getHandleGPU();
}

CUTE_NS_END
