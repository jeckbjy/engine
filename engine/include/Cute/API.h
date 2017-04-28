#pragma once

// define CUTE_CORE_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_CORE)
// build or use lib
#	define CUTE_CORE_API
#elif defined(CUTE_BUILD_CORE)
// build dynamic dll
#	define CUTE_CORE_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_CORE_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_CORE) && !defined(CUTE_UNAUTOLINK_CORE)
#	pragma comment(lib, "engine.lib")
#endif

#if defined(_MSC_VER)
#	pragma comment(lib, "ws2_32.lib")
#endif
