#pragma once
#include "VK_API.h"

CU_NS_BEGIN

class VK_Device;
class CU_VK_API VK_Shader : public ShaderStage
{
public:
	VK_Shader(VK_Device* device);
	~VK_Shader();

	bool compile(const ShaderDesc& desc);

private:
	VK_Device*		m_device;
	VkShaderModule	m_module;
};

CU_NS_END
