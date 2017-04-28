#pragma once
#include "Cute/API.h"

// define CUTE_PTSQL_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_PTSQL)
// build or use lib
#	define CUTE_PTSQL_API
#elif defined(CUTE_BUILD_PTSQL)
// build dynamic dll
#	define CUTE_PTSQL_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_PTSQL_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_PTSQL) && !defined(CUTE_UNAUTOLINK_PTSQL)
#	pragma comment(lib, "plugin_ptsql.lib")
#endif

#define PTSQL_NAME "postgre"