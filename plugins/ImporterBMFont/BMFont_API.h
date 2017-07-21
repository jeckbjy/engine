#pragma once

#if defined(CUTE_USE_LIB_BMFONT)
// build or use lib
#	define CUTE_BMFONT_API
#elif defined(CUTE_BUILD_BMFONT)
// build dynamic dll
#	define CUTE_BMFONT_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_BMFONT_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_BMFONT) && !defined(CUTE_UNAUTOLINK_BMFONT)
#	pragma comment(lib, "plugin_font.lib")
#endif
