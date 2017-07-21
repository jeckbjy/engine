#pragma once

// define CUTE_FONT_API
#if defined(CUTE_USE_LIB_FREETYPE)
// build or use lib
#	define CUTE_FREETYPE_API
#elif defined(CUTE_BUILD_FREETYPE)
// build dynamic dll
#	define CUTE_FREETYPE_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FREETYPE_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FREETYPE) && !defined(CUTE_UNAUTOLINK_FREETYPE)
#	pragma comment(lib, "plugin_freetype.lib")
#endif
