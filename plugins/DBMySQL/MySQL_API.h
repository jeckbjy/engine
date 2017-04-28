#pragma once

// define CUTE_MYSQL_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_MYSQL)
// build or use lib
#	define CUTE_MYSQL_API
#elif defined(CUTE_BUILD_MYSQL)
// build dynamic dll
#	define CUTE_MYSQL_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_MYSQL_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_MYSQL) && !defined(CUTE_UNAUTOLINK_MYSQL)
#	pragma comment(lib, "plugin_mysql.lib")
#endif

#define MYSQL_NAME "mysql"
