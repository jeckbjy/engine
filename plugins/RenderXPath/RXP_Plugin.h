#pragma once
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class RXP_Plugin : public Plugin
{
public:
    RXP_Plugin();

	void install();
	void uninstall();
};

CUTE_NS_END
