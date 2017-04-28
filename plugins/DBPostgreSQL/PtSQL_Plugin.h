#pragma once
#include "PtSQL_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_PTSQL_API PtSQLPlugin : public Plugin
{
public:
	PtSQLPlugin() : Plugin(PTSQL_NAME){}

	void install();
	void uninstall();
};

CUTE_NS_END
