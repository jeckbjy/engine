#include "DynLib.h"

#ifdef CU_OS_WIN
#	include <Windows.h>
#	define DYNLIB_SUFFIX ".dll"
#	define DYNLIB_LOAD(a)			LoadLibraryEx(a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#	define DYNLIB_UNLOAD(a)			!FreeLibrary(a)
#	define DYNLIB_GETSYM(a, b)		GetProcAddress(a,b)
#elif defined(CU_OS_MAC)
#include <dlfcn.h>
#	define DYNLIB_SUFFIX ".dylib"
#	define DYNLIB_LOAD(a)			mac_loadDylib(a)
#	define DYNLIB_UNLOAD(a)			dlclose(a)
#	define DYNLIB_GETSYM(a, b)		dlsym(a,b)
#else
#include <dlfcn.h>
#	define DYNLIB_SUFFIX ".so"
#	define DYNLIB_LOAD( a )			dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#	define DYNLIB_UNLOAD( a )		dlclose( a )
#	define DYNLIB_GETSYM( a, b)		dlsym( a, b )
#endif

CU_NS_BEGIN

const String& DynLib::suffix()
{
	static String ss(DYNLIB_SUFFIX);
	return ss;
}

String DynLib::checkPath(const String& path)
{
	const String& suf = suffix();
	if (path.size() > suf.size() && strcmp(path.data() + path.size() - suf.size(), suf.c_str()) == 0)
		return path;
	return path + suf;
}

DynLib::DynLib()
	: m_handle(0)
	, m_plugin(NULL)
{
}

DynLib::~DynLib()
{
	if (m_plugin)
	{
		m_plugin->release();
		m_plugin = NULL;
	}

	unload();
}

bool DynLib::load(const String& path)
{
	m_handle = DYNLIB_LOAD(path.c_str());
	if (!m_handle)
	{
		String realPath = path + suffix();
		m_handle = DYNLIB_LOAD(realPath.c_str());
	}

	return m_handle != NULL;
}

void DynLib::unload()
{
	if (m_handle)
	{
		if (DYNLIB_UNLOAD(m_handle))
			throw std::runtime_error("unload error");
		m_handle = 0;
	}
}

void* DynLib::getSymbol(const String& name)
{
	return (void*)DYNLIB_GETSYM(m_handle, name.c_str());
}

bool DynLib::isLoaded() const 
{
	return m_handle != 0; 
}

Plugin* DynLib::loadPlugin()
{
	if (m_handle && !m_plugin)
	{
		PluginMainFun fun = (PluginMainFun)getSymbol(DLL_PLUGIN_MAIN_NAME);
		if (fun != NULL)
		{
			m_plugin = fun();
		}
	}

	return m_plugin;
}

CU_NS_END
