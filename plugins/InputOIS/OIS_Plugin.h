#pragma once
#include "OIS_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define OIS_NAME "ObjectOrientedInputSystem"

class CUTE_OIS_API OISPlugin : public Plugin
{
public:
	OISPlugin() :Plugin(OIS_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
