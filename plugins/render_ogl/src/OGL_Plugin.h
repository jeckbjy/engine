#pragma once
#include "Plugin.h"

CU_NS_BEGIN

#define OGL_NAME "OGL"
class OGLPlugin : public Plugin
{
public:
	OGLPlugin():Plugin(OGL_NAME){}

	void install();
	void uninstall();
};

CU_NS_END
