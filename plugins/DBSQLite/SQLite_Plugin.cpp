#include "SQLite_Plugin.h"

CUTE_NS_BEGIN

DEF_PLUGIN(CUTE_SQLITE_API, SQLitePlugin)

void SQLitePlugin::install()
{
	//SQLManager::Instance().add(new MySQLFactory());
}

void SQLitePlugin::uninstall()
{
	//SQLManager::Instance().remove(MYSQL_NAME);
}

CUTE_NS_END
