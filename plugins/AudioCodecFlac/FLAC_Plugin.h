#pragma once
#include "FLAC_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define FLAC_NAME "FLAC"

class CUTE_FLAC_API FLACPlugin : public Plugin
{
public:
	FLACPlugin() :Plugin(FLAC_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
