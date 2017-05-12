#pragma once
#include "PhysX_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define PHYSX_NAME "PhysX"

class CUTE_PHYSX_API PhysXPlugin : public Plugin
{
public:
	PhysXPlugin() :Plugin(PHYSX_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
