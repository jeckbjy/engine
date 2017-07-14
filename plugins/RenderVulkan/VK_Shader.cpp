#include "VK_Shader.h"
#include "VK_Device.h"

CUTE_NS_BEGIN

VK_Shader::VK_Shader()
	: m_device(NULL)
	, m_module(VK_NULL_HANDLE)
{
}

VK_Shader::~VK_Shader()
{
}

bool VK_Shader::init(VK_Device* device)
{
	m_device = device;
	return true;
}

void VK_Shader::term()
{
	if (m_module != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(m_device->getDevice(), m_module, NULL);
		m_module = VK_NULL_HANDLE;
	}
}

bool VK_Shader::compile(const ShaderDesc& desc)
{
	VkDevice device;
	VkResult result;

	device = m_device->getDevice();
	// 支持宏定义么,如何进行反射？？
	VkShaderModuleCreateInfo info;
	info.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pNext	= NULL;

	info.flags		= 0;
	info.codeSize	= desc.codeSize;
	info.pCode		= static_cast<const uint32_t*>(desc.code);

	result = vkCreateShaderModule(device, &info, NULL, &m_module);
	return result == VK_SUCCESS;
}

CUTE_NS_END
