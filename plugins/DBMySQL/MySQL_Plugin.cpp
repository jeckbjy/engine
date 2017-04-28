#include "MySQL_Plugin.h"

CUTE_NS_BEGIN

// 导出入口
DEF_PLUGIN(CUTE_MYSQL_API, MySQLPlugin)

void MySQLPlugin::install()
{
	//SQLManager::Instance().add(new MySQLFactory());
}

void MySQLPlugin::uninstall()
{
	//SQLManager::Instance().remove(MYSQL_NAME);
}

CUTE_NS_END
