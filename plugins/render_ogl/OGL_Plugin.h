#pragma once
#include "Plugin.h"

CU_NS_BEGIN

#define OGL_NAME "OGL"
class OGL_Plugin : public Plugin
{
public:
	OGL_Plugin():Plugin(OGL_NAME){}

	void install();
	void uninstall();
};

CU_NS_END
