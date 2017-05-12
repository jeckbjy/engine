#pragma once

// define CUTE_FONT_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_FONT)
// build or use lib
#	define CUTE_FONT_API
#elif defined(CUTE_BUILD_FONT)
// build dynamic dll
#	define CUTE_FONT_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FONT_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FONT) && !defined(CUTE_UNAUTOLINK_FONT)
#	pragma comment(lib, "plugin_font.lib")
#endif
