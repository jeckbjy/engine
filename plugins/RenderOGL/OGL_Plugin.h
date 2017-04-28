#pragma once
#include "Plugin.h"

CUTE_NS_BEGIN

#define OGL_NAME "OGL"
class OGL_Device;
class OGL_Plugin : public Plugin
{
public:
	OGL_Plugin();
	~OGL_Plugin();

	void install();
	void uninstall();

private:
	OGL_Device* m_device;
};

CUTE_NS_END
