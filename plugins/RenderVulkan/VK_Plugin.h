#pragma once
#include "Plugin.h"

CUTE_NS_BEGIN

#define VULKAN_NAME "Vulkan"
// ������
class VK_Plugin : public Plugin
{
public:
	VK_Plugin():Plugin(VULKAN_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
