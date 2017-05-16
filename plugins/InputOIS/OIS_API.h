#pragma once

// define CUTE_OIS_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_OIS)
// build or use lib
#	define CUTE_OIS_API
#elif defined(CUTE_BUILD_OIS)
// build dynamic dll
#	define CUTE_OIS_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_OIS_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_OIS) && !defined(CUTE_UNAUTOLINK_OIS)
#	pragma comment(lib, "plugin_ois.lib")
#endif

#include <OIS/OIS.h>
