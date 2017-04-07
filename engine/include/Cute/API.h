#pragma once

// define CUTE_CORE_API
#if defined (_WIN32) && defined(_DLL)
#	if !defined(CUTE_DLL) && !defined(CUTE_STATIC)
#	define CUTE_DLL
#	endif
#endif

#if defined(CUTE_DLL)
#	if defined(CUTE_BUILD_DLL)
#	define CUTE_CORE_API CUTE_EXPORT
#	else
#	define CUTE_CORE_API CUTE_IMPORT
#	endif
#else
#	define CUTE_CORE_API
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER)
#	if !defined(CUTE_NO_AUTO_LINK) && !defined(CUTE_BUILD_DLL)
#	pragma comment(lib, "engine.lib")
#	endif
#endif

#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif
