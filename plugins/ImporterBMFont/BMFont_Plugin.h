#pragma once
#include "BMFont_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_BMFONT_API BMFontPlugin : public Plugin
{
public:
    BMFontPlugin();

	void install();
	void uninstall();
};

CUTE_NS_END
