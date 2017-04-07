// module SharedLibrary
#include "Cute/SharedLibrary.h"
#include "Cute/Unicode.h"
#include "Cute/Path.h"

#ifdef CUTE_OS_FAMILY_WINDOWS
#	define DYNLIB_UNLOAD(a)			FreeLibrary(a)
#	define DYNLIB_GETSYM(a, b)		GetProcAddress(a,b)
#elif (CUTE_OS == CUTE_OS_HPUX)
#	define DYNLIB_UNLOAD(a)			shl_unload(a)
#	define DYNLIB_GETSYM(a, b)		shl_findsym(a,b)
#else
#	include <dlfcn.h>
#	define DYNLIB_UNLOAD( a )		dlclose( a )
#	define DYNLIB_GETSYM( a, b)		dlsym( a, b )
#endif

#if CUTE_OS == CUTE_OS_CYGWIN && !defined(RTLD_LOCAL)
#define RTLD_LOCAL 0
#endif

CUTE_NS_BEGIN

Mutex SharedLibrary::s_mutex;

SharedLibrary::SharedLibrary()
	: m_handle(0)
{
}

SharedLibrary::SharedLibrary(const String& path)
	: m_handle(0)
{
	load(path);
}

SharedLibrary::SharedLibrary(const String& path, int flags)
	: m_handle(0)
{
	load(path, flags);
}

SharedLibrary::~SharedLibrary()
{
}

void SharedLibrary::load(const String& path)
{
	load(path, 0);
}

void SharedLibrary::load(const String& path, int flags /* = SHLIB_DEFAULT */)
{
	Mutex::ScopedLock lock(s_mutex);
	if (m_handle)
		throw LibraryAlreadyLoadedException(path);

#if defined (CUTE_OS_FAMILY_WINDOWS)
	// 需要转换成Unicode字符
	DWORD readFlag(0);
	Path p(path);
	if (p.isAbsolute())
		flags |= LOAD_WITH_ALTERED_SEARCH_PATH;
	m_handle = LoadLibraryExA(path.c_str(), 0, flags);

#elif (CUTE_OS == CUTE_OS_HPUX)
	m_handle = shl_load(path.c_str(), BIND_DEFERRED, 0);
#else
	int realFlags = RTLD_LAZY;
	if (flags & SHLIB_LOCAL_IMPL)
		realFlags |= RTLD_LOCAL;
	else
		realFlags |= RTLD_GLOBAL;
	m_handle = dlopen(path.c_str(), realFlags);
	if (!m_handle)
	{
		const char* err = dlerror();
		throw LibraryLoadException(err ? std::string(err) : path);
	}
#endif

	if (!m_handle)
		throw LibraryLoadException(path);

	m_path = path;
}

void SharedLibrary::unload()
{
	Mutex::ScopedLock lock(s_mutex);

	if (m_handle)
	{
		DYNLIB_UNLOAD(m_handle);
		m_handle = 0;
	}

	m_path.clear();
}

bool SharedLibrary::isLoaded() const
{
	return m_handle != 0;
}

bool SharedLibrary::hasSymbol(const String& name)
{
	return getSymbol(name) != 0;
}

void* SharedLibrary::getSymbol(const String& name)
{
	Mutex::ScopedLock lock(s_mutex);

	if (m_handle)
		return (void*)DYNLIB_GETSYM(m_handle, name.c_str());

	return 0;
}

const String& SharedLibrary::prefix()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	static const String pre = "";
#elif (CUTE_OS == CUTE_OS_CYGWIN)
	static const String pre = "cyg";
#else
	static const String pre = "lib";
#endif

	return pre;
}

const String& SharedLibrary::suffix()
{
#if defined(CUTE_OS_FAMILY_WINDOWS) || (CUTE_OS == CUTE_OS_CYGWIN)
	static const String suf = ".dll";
#elif (CUTE_OS == CUTE_OS_MAC_OS_X)
	static const String suf = ".dylib";
#elif (CUTE_OS == CUTE_OS_HPUX)
	static const String suf = ".sl";
#else
	static const String suf = ".so";
#endif

	return suf;
}

String SharedLibrary::getOSName(const String& name)
{
	return prefix() + name + suffix();
}

CUTE_NS_END