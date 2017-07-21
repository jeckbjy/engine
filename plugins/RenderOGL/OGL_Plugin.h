#pragma once
#include "OGL_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define OGL_NAME "OGL"
class OGL_Device;
class CUTE_OGL_API OGL_Plugin : public Plugin
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
