#pragma once
#include "OAL_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define OAL_NAME "OpenAL"

class CUTE_OAL_API OALPlugin : public Plugin
{
public:
	OALPlugin() :Plugin(OAL_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
