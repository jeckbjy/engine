#pragma once
#include "FBX_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define FBX_NAME "FBX"

class CUTE_FBX_API FBXPlugin : public Plugin
{
public:
	FBXPlugin() :Plugin(FBX_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
