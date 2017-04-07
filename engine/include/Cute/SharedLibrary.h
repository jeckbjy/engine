#pragma once
#include "Cute/Foundation.h"
#include "Cute/Mutex.h"

#if defined(CUTE_OS_FAMILY_WINDOWS)
typedef HMODULE		module_t;
#elif (CUTE_OS == CUTE_OS_HPUX)
#include <dl.h>
typedef shl_t		module_t;
#else
typedef void*		module_t;
#endif

CUTE_NS_BEGIN

class CUTE_CORE_API SharedLibrary
{
public:
	enum Flags
	{
		/// On platforms that use dlopen(), use RTLD_GLOBAL. This is the default
		/// if no flags are given.
		///
		/// This flag is ignored on platforms that do not use dlopen().
		SHLIB_GLOBAL = 1,

		/// On platforms that use dlopen(), use RTLD_LOCAL instead of RTLD_GLOBAL.
		///
		/// Note that if this flag is specified, RTTI (including dynamic_cast and throw) will
		/// not work for types defined in the shared library with GCC and possibly other
		/// compilers as well. See http://gcc.gnu.org/faq.html#dso for more information.
		///
		/// This flag is ignored on platforms that do not use dlopen().
		SHLIB_LOCAL = 2
	};

	SharedLibrary();
	SharedLibrary(const String& path);
	SharedLibrary(const String& path, int flags);
	virtual ~SharedLibrary();

	/// Loads a shared library from the given path,
	/// using the given flags. See the Flags enumeration
	/// for valid values.
	/// Throws a LibraryAlreadyLoadedException if
	/// a library has already been loaded.
	/// Throws a LibraryLoadException if the library
	/// cannot be loaded.
	void load(const String& path);
	void load(const String& path, int flags);
	void unload();
	bool isLoaded() const;

	bool hasSymbol(const String& name);
	void* getSymbol(const String& name);


	const String& getPath() const;

	/// Returns the platform-specific filename prefix
	/// for shared libraries.
	/// Most platforms would return "lib" as prefix, while
	/// on Cygwin, the "cyg" prefix will be returned.
	static const String& prefix();

	/// Returns the platform-specific filename suffix
	/// for shared libraries (including the period).
	/// In debug mode, the suffix also includes a
	/// "d" to specify the debug version of a library.
	static const String& suffix();

	/// Returns the platform-specific filename 
	/// for shared libraries by prefixing and suffixing name
	/// with prefix() and suffix()
	static String getOSName(const String& name);

private:
	SharedLibrary(const SharedLibrary&);
	SharedLibrary& operator = (const SharedLibrary&);

private:
	String	 m_path;
	module_t m_handle;
	static Mutex s_mutex;
};

CUTE_NS_END
