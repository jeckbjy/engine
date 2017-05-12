#pragma once
#include "FMOD_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define FMOD_NAME "FMOD"

class CUTE_FMOD_API FMODPlugin : public Plugin
{
public:
	FMODPlugin() :Plugin(FMOD_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
