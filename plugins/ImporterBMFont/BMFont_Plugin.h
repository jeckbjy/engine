#pragma once
#include "BMFont_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_BMFONT_API BMFont_Plugin : public Plugin
{
public:
    BMFont_Plugin();

	void install();
	void uninstall();
};

CUTE_NS_END
