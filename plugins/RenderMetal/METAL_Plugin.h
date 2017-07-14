#pragma once
#include "METAL_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define METAL_NAME "Metal"

class CUTE_METAL_API METAL_Plugin : public Plugin
{
public:
	METAL_Plugin();
	~METAL_Plugin();

	void install();
	void uninstall();
};

CUTE_NS_END
