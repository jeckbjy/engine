#pragma once

// define CUTE_SQLITE_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_SQLITE)
// build or use lib
#	define CUTE_SQLITE_API
#elif defined(CUTE_BUILD_SQLITE)
// build dynamic dll
#	define CUTE_SQLITE_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_SQLITE_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_SQLITE) && !defined(CUTE_UNAUTOLINK_SQLITE)
#	pragma comment(lib, "plugin_sqlite.lib")
#endif

#define SQLITE_NAME "sqlite"
