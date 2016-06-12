#include "VK_Shader.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_Shader::VK_Shader(VK_Device* device)
	: m_device(device)
	, m_module(VK_NULL_HANDLE)
{
}

VK_Shader::~VK_Shader()
{
	if (m_module != VK_NULL_HANDLE)
		vkDestroyShaderModule(m_device->native(), m_module, NULL);
}

bool VK_Shader::compile(const ShaderDesc& desc)
{
	// 支持宏定义么,如何进行反射？？
	VkShaderModuleCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pNext = NULL;

	info.flags = 0;
	info.codeSize = desc.code.size();
	info.pCode = (uint32_t*)desc.code.c_str();

	VK_CHECK(vkCreateShaderModule(m_device->native(), &info, NULL, &m_module), "vkCreateShaderModule fail!");
	return true;
}

CU_NS_END
