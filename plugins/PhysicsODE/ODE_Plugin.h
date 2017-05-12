#pragma once
#include "ODE_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define ODE_NAME "OpenDynamicEngine"

class CUTE_ODE_API ODEPlugin : public Plugin
{
public:
	ODEPlugin() :Plugin(ODE_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
