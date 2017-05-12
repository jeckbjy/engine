#pragma once
#include "VK_API.h"

CUTE_NS_BEGIN

class VK_Device;
class CUTE_VK_API VK_Shader : public IShaderModule
{
public:
	VK_Shader();
	~VK_Shader();

	bool init(VK_Device* device);
	void term();

	bool compile(const ShaderDesc& desc);

private:
	VK_Device*		m_device;
	VkShaderModule	m_module;
};

CUTE_NS_END
