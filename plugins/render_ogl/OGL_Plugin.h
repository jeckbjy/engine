#pragma once
#include "Plugin.h"

CU_NS_BEGIN

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

CU_NS_END
