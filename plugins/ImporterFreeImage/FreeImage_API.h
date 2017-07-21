#pragma once

// define CUTE_FREEIMAGE_API
#if defined(CUTE_USE_LIB_FREEIMAGE)
// build or use lib
#	define CUTE_FREEIMAGE_API
#elif defined(CUTE_BUILD_FREEIMAGE)
// build dynamic dll
#	define CUTE_FREEIMAGE_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FREEIMAGE_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FREEIMAGE) && !defined(CUTE_UNAUTOLINK_FREEIMAGE)
#	pragma comment(lib, "plugin_freeimage.lib")
#endif
