#include "PtSQL_Plugin.h"

CUTE_NS_BEGIN

// �������
DEF_PLUGIN(CUTE_PTSQL_API, PtSQLPlugin)

void PtSQLPlugin::install()
{
	//SQLManager::Instance().add(new MySQLFactory());
}

void PtSQLPlugin::uninstall()
{
	//SQLManager::Instance().remove(MYSQL_NAME);
}

CUTE_NS_END
