#pragma once
#include "Cute/Table.h"
using namespace Cute;

struct ItemCfg : public IRow
{
	int type;
	void decode(CellVector& cells)
	{
		cells >> type;
	}
};

typedef Table<ItemCfg> ItemTab;

extern ItemTab gItemTab;

class GameConfigMgr
{
public:
	GameConfigMgr(){}
	~GameConfigMgr(){}

	void load();
	bool reload(const String& filename);

public:
	typedef std::map<String, ITable*> TableMap;
	TableMap m_tables;
};
