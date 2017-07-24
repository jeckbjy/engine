#pragma once
#include "FreeType_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_FREETYPE_API FreeType_Plugin : public Plugin
{
public:
    FreeType_Plugin();

	void install();
	void uninstall();
};

CUTE_NS_END
