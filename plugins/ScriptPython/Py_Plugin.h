#pragma once
#include "Py_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_PY_API PyPlugin : public Plugin
{
public:
	PyPlugin() : Plugin(PY_PLUGIN_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END

