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

String& DynLib::suffix()
{
	static String ss(DYNLIB_SUFFIX);
	return ss;
}

String DynLib::check_path(const String& path)
{
	const String& suf = suffix();
	if (path.size() > suf.size() && strcmp(path.data() + path.size() - suf.size(), suf.c_str()) == 0)
		return path;
	return path + suf;
}

DynLib::DynLib() :_handle(0){}
DynLib::~DynLib(){ unload(); }

bool DynLib::load(const String& path)
{
	_handle = DYNLIB_LOAD(path.c_str());
	return _handle != NULL;
}

void DynLib::unload()
{
	if (_handle)
	{
		if (DYNLIB_UNLOAD(_handle))
			throw std::runtime_error("unload error");
		_handle = 0;
	}
}

void* DynLib::getSymbol(const String& name)
{
	return (void*)DYNLIB_GETSYM(_handle, name.c_str());
}

bool DynLib::isLoaded() const { return _handle != 0; }

CU_NS_END