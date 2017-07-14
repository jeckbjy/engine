#pragma once
#include "Plugin.h"

CUTE_NS_BEGIN

#define VULKAN_NAME "Vulkan"
// ²å¼þÈë¿Ú
class VK_Plugin : public Plugin
{
public:
	VK_Plugin():Plugin(VULKAN_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
