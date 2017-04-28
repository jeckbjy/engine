#pragma once
#include "Cute/API.h"

// define CUTE_PTSQL_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
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