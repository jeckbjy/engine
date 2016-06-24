#include "PluginMgr.h"

CU_NS_BEGIN

PluginMgr::PluginMgr()
{

}

PluginMgr::~PluginMgr()
{
	for (PluginMap::iterator itor = m_plugins.begin(); itor != m_plugins.end(); ++itor)
	{
		itor->second->uninstall();
	}

	for (LibMap::iterator itor = m_libs.begin(); itor != m_libs.end(); ++itor)
	{
		delete (itor->second);
	}
	m_plugins.clear();
	m_libs.clear();
}

void PluginMgr::load(const String& path, bool install /* = true */)
{
	LibMap::iterator itor = m_libs.find(path);
	if (itor != m_libs.end())
		return;
	DynLib* lib = new DynLib();
	lib->load(path);
	Plugin* plugin = lib->loadPlugin();

	if (plugin)
	{
		if (install)
		{
			plugin->install();
		}

		m_plugins[plugin->name()] = plugin;
	}

	m_libs[path] = lib;
}

void PluginMgr::unload(const String& path)
{
	LibMap::iterator itor = m_libs.find(path);
	if (itor != m_libs.end())
	{
		DynLib* lib = itor->second;
		Plugin* plugin = lib->getPlugin();
		if (plugin)
		{
			m_plugins.erase(plugin->name());
		}

		delete lib;
		m_libs.erase(itor);
	}
}

DynLib* PluginMgr::getDynLib(const String& path)
{
	LibMap::iterator itor = m_libs.find(path);
	if (itor != m_libs.end())
		return itor->second;
	return NULL;
}

Plugin* PluginMgr::getPlugin(const String& name)
{
	PluginMap::iterator itor = m_plugins.find(name);
	if (itor != m_plugins.end())
		return itor->second;

	return NULL;
}

CU_NS_END
