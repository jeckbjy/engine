#pragma once
#include "Cute/Plugin.h"
#include "MySQL_API.h"

CUTE_NS_BEGIN

// ²å¼þÈë¿Ú
class CUTE_MYSQL_API MySQLPlugin : public Plugin
{
public:
	MySQLPlugin() :Plugin(MYSQL_NAME){}
	void install();
	void uninstall();
};

CUTE_NS_END
