#pragma once
#include "Cute/Foundation.h"
#include "Cute/Mutex.h"
#include "Cute/Singleton.h"

CUTE_NS_BEGIN

class Plugin;
class SharedLibrary;

class CUTE_CORE_API PluginManager : public Singleton<PluginManager>
{
	struct Data;
public:
	PluginManager();
	~PluginManager();

	void load(const String& libpath);
	void load(const String& path, const String& name);
	bool unloadLibrary(const String& name);
	bool unloadPlugin(const String& name);
	void unloadAll();

protected:
	void unload(Data* data);

private:
	struct Data
	{
		SharedLibrary*	lib;
		Plugin*			plugin;
		String			libname;
	};

	typedef std::map<String, Data*> PluginMap;
	typedef std::map<String, Data*> Librarymap;
	typedef Plugin* (*PLUGIN_MAIN_FUN)(void);

	Mutex		m_mutex;
	PluginMap	m_plugins;
	Librarymap	m_libs;
};

CUTE_NS_END
