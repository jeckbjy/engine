#include "VK_Program.h"

CU_NS_BEGIN

VK_Program::VK_Program(VK_Device* device, const PROGRAM_DESC& desc)
	: m_device(device)
	, m_module(VK_NULL_HANDLE)
{
	// 支持宏定义么,如何进行反射？？
	VkShaderModuleCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pNext = NULL;

	info.flags = 0;
	info.codeSize = 0;
	info.pCode = NULL;

	VK_CHECK(vkCreateShaderModule(*m_device, &info, NULL, &m_module), "vkCreateShaderModule fail!");
}

VK_Program::~VK_Program()
{
	if (m_module != VK_NULL_HANDLE)
		vkDestroyShaderModule(*m_device, m_module, NULL);
}

CU_NS_END
