//! Config
#include "GameConfigMgr.h"
#include <iostream>
using namespace std;

ItemTab gItemTab;

void GameConfigMgr::load()
{
	for (ItemTab::Iterator itor = gItemTab.begin(); itor != gItemTab.end(); ++itor)
	{
		cout << itor->type << endl;
	}
}

bool GameConfigMgr::reload(const String& filename)
{
	TableMap::iterator itor = m_tables.find(filename);
	if (itor != m_tables.end())
	{
		itor->second->reload();
		return true;
	}

	return false;
}
