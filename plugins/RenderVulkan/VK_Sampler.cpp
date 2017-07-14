#include "VK_Sampler.h"
#include "VK_Device.h"
#include "VK_Mapping.h"

CUTE_NS_BEGIN

VK_Sampler::VK_Sampler()
	: m_device(NULL)
	, m_sampler(VK_NULL_HANDLE)
{
}

VK_Sampler::~VK_Sampler()
{
	term();
}

bool VK_Sampler::init(VK_Device* device, const SamplerDesc& desc)
{
	m_device = device;
	m_device->incRef();

	VkDevice native = device->getDevice();

	VkSamplerCreateInfo info = {};
    info.sType                      = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext                      = NULL;
    info.flags                      = 0;
    info.magFilter                  = VK_Mapping::getFilter(desc.magFilter);
    info.minFilter                  = VK_Mapping::getFilter(desc.minFilter);
    info.mipmapMode                 = VK_Mapping::getMipmapMode(desc.mipmapMode);
    info.addressModeU               = VK_Mapping::getAddressMode(desc.addressU);
    info.addressModeV               = VK_Mapping::getAddressMode(desc.addressV);
    info.addressModeW               = VK_Mapping::getAddressMode(desc.addressW);
    info.mipLodBias                 = desc.mipLodBias;
    info.anisotropyEnable           = desc.anisotropyEnable;
	info.maxAnisotropy				= (float)desc.maxAnisotropy;
    info.compareEnable              = desc.compareEnable;
    info.compareOp                  = VK_Mapping::getCompareOp(desc.compareOp);
    info.minLod                     = desc.minLod;
    info.maxLod                     = desc.maxLod;
    info.borderColor                = VK_Mapping::getBorderColor(desc.borderColor);
    info.unnormalizedCoordinates    = VK_FALSE;

	VkResult result = vkCreateSampler(native, &info, NULL, &m_sampler);
	if (result != VK_SUCCESS)
		return false;

	return true;
}

void VK_Sampler::term()
{
	if (m_device == NULL)
		return;
	
	VkDevice device = m_device->getDevice();
	
	vkDestroySampler(device, m_sampler, NULL);
	m_sampler = VK_NULL_HANDLE;

	m_device->decRef();
	m_device = NULL;
}

CUTE_NS_END
