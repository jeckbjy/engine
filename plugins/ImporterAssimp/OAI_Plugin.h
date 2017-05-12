#pragma once
#include "OAI_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define OAI_NAME "OpenAssetImporter"

class CUTE_OAI_API OAIPlugin : public Plugin
{
public:
	OAIPlugin() :Plugin(OAI_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
