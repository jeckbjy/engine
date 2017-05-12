#pragma once
#include "Font_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define FONT_NAME "Font"

class CUTE_FONT_API FontPlugin : public Plugin
{
public:
	FontPlugin() :Plugin(FONT_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
