#pragma once
#include "DynLib.h"
#include "Plugin.h"

CU_NS_BEGIN

class CU_API PluginMgr
{
public:
	PluginMgr();
	~PluginMgr();

	void load(const String& path, bool install = true);
	void unload(const String& path);
	DynLib* getDynLib(const String& path);
	Plugin* getPlugin(const String& name);

private:
	typedef std::map<String, DynLib*> LibMap;
	typedef std::map<String, Plugin*> PluginMap;
	LibMap		m_libs;
	PluginMap	m_plugins;
};

CU_NS_END
