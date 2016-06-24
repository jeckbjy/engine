#pragma once
#include "API.h"
#include "Plugin.h"

CU_NS_BEGIN

// dll¶¯Ì¬Á´½Ó¿â
class CU_API DynLib
{
	CU_NONCOPYABLE(DynLib);
public:
	typedef void(*DllFun)(void);
	static const String& suffix();// ºó×º
	static String checkPath(const String& path);
public:
	DynLib();
	~DynLib();

	bool load(const String& path);
	void unload();
	bool isLoaded() const;
	void* getSymbol(const String& name);

	Plugin* loadPlugin();
	Plugin* getPlugin() { return m_plugin; }

private:
	module_t m_handle;
	Plugin*	 m_plugin;
};

CU_NS_END