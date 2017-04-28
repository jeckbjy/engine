#pragma once
#include "V8_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_V8_API V8Plugin : public Plugin
{
public:
	V8Plugin() : Plugin(V8_PLUGIN_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END

