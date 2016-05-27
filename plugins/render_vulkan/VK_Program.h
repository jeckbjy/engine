#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class VK_Device;
class CU_VK_API VK_Program : public ShaderStage
{
public:
	VK_Program(VK_Device* device);
	~VK_Program();

	bool compile(const ProgramDesc& desc);

private:
	VK_Device*		m_device;
	VkShaderModule	m_module;
};

CU_NS_END
