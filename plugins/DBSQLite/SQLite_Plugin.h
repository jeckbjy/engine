#pragma once
#include "SQLite_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

class CUTE_SQLITE_API SQLitePlugin : public Plugin
{
public:
	SQLitePlugin() :Plugin(SQLITE_NAME){}
	void install();
	void uninstall();
};

CUTE_NS_END
