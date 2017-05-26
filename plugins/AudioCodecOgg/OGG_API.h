#pragma once

// define CUTE_OGG_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_OGG)
// build or use lib
#	define CUTE_OGG_API
#elif defined(CUTE_BUILD_OGG)
// build dynamic dll
#	define CUTE_OGG_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_OGG_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_OGG) && !defined(CUTE_UNAUTOLINK_OGG)
#	pragma comment(lib, "plugin_ogg.lib")
#endif
