#pragma once

// define CUTE_FLAC_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_FLAC)
// build or use lib
#	define CUTE_FLAC_API
#elif defined(CUTE_BUILD_FLAC)
// build dynamic dll
#	define CUTE_FLAC_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FLAC_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FLAC) && !defined(CUTE_UNAUTOLINK_FLAC)
#	pragma comment(lib, "plugin_flac.lib")
#endif
