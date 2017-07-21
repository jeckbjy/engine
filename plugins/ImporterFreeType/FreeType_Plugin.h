#pragma once
#include "FreeType_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_FREETYPE_API FreeTypePlugin : public Plugin
{
public:
    FreeTypePlugin();

	void install();
	void uninstall();
};

CUTE_NS_END
