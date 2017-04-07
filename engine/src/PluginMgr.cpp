//! SharedLibrary
#include "Cute/PluginMgr.h"
#include "Cute/Plugin.h"
#include "Cute/Path.h"
#include "Cute/String.h"
#include "Cute/SharedLibrary.h"

CUTE_NS_BEGIN

PluginMgr::PluginMgr()
{

}

PluginMgr::~PluginMgr()
{
	unloadAll();
}

void PluginMgr::load(const String& path)
{
	Path p(path);
	String libname = p.getFileName();

	Mutex::ScopedLock lock(m_mutex);
	if (m_libs.find(libname) != m_libs.end())
		throw ExistsException(path);

	SharedLibrary* lib = new SharedLibrary();
	try
	{
		if (!endsWith(path, SharedLibrary::suffix()))
			lib->load(path + SharedLibrary::suffix());
		else
			lib->load(path);

		PLUGIN_MAIN_FUN fun = (PLUGIN_MAIN_FUN)lib->getSymbol(PLUGIN_MAIN_NAME);
		if (fun == 0)
		{
			delete lib;
			throw SystemException("donot find PluginMain", path);
		}

		Plugin* plg = fun();
		if (plg == NULL)
		{
			delete lib;
			throw SystemException("create plugin fail!", path);
		}

		if (m_plugins.find(plg->name()) != m_plugins.end())
		{
			plg->release();
			delete lib;
			throw SystemException("duplate plugin", path);
		}

		plg->install();
		Data* data = new Data();
		data->plugin = plg;
		data->lib = lib;
		data->libname = libname;
		m_plugins[plg->name()] = data;
		m_libs[libname] = data;
	}
	catch (...)
	{
		delete lib;
	}
	
}

void PluginMgr::load(const String& path, const String& name)
{
	String filename = path + SharedLibrary::getOSName(name);
	load(filename);
}

bool PluginMgr::unloadLibrary(const String& name)
{
	Mutex::ScopedLock lock(m_mutex);
	Librarymap::iterator itor = m_libs.find(name);
	if (itor == m_libs.end())
		return false;

	Data* data = itor->second;
	m_libs.erase(itor);
	m_plugins.erase(data->plugin->name());

	unload(data);
	return true;
}

bool PluginMgr::unloadPlugin(const String& name)
{
	Mutex::ScopedLock lock(m_mutex);
	Librarymap::iterator itor = m_plugins.find(name);
	if (itor == m_plugins.end())
		return false;

	Data* data = itor->second;

	m_libs.erase(data->libname);
	m_plugins.erase(itor);

	unload(data);
	return true;
}

void PluginMgr::unloadAll()
{
	Mutex::ScopedLock lock(m_mutex);

	for (PluginMap::iterator itor = m_plugins.begin(); itor != m_plugins.end(); ++itor)
	{
		Data* data = itor->second;
		unload(data);
	}
	m_plugins.clear();
	m_libs.clear();
}

void PluginMgr::unload(Data* data)
{
	SharedLibrary*	lib = data->lib;
	Plugin*			plg = data->plugin;

	plg->uninstall();
	lib->unload();

	plg->release();
	delete lib;
	delete data;
}

CUTE_NS_END