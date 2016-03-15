#include "VK_Program.h"
#include "VK_Device.h"

CU_NS_BEGIN

VK_Program::VK_Program(VK_Device* device)
	: m_device(device)
	, m_module(VK_NULL_HANDLE)
{
}

VK_Program::~VK_Program()
{
	if (m_module != VK_NULL_HANDLE)
		vkDestroyShaderModule(m_device->native(), m_module, NULL);
}

bool VK_Program::compile(const ProgramDesc& desc)
{
	// ֧�ֺ궨��ô,��ν��з��䣿��
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
