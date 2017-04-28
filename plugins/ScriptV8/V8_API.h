#pragma once

// define CUTE_PY_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_V8)
// build or use lib
#	define CUTE_V8_API
#elif defined(CUTE_BUILD_V8)
// build dynamic dll
#	define CUTE_V8_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_V8_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_V8) && !defined(CUTE_UNAUTOLINK_V8)
#	pragma comment(lib, "plugin_v8.lib")
#endif

#define V8_PLUGIN_NAME "v8"
