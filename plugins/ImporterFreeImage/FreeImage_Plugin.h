#pragma once
#include "FreeImage_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define FREEIMAGE_NAME "FreeImage"

class CUTE_FREEIMAGE_API FreeImagePlugin : public Plugin
{
public:
	FreeImagePlugin() :Plugin(FREEIMAGE_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
