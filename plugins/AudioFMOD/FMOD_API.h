#pragma once

// define CUTE_FMOD_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_FMOD)
// build or use lib
#	define CUTE_FMOD_API
#elif defined(CUTE_BUILD_FMOD)
// build dynamic dll
#	define CUTE_FMOD_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FMOD_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FMOD) && !defined(CUTE_UNAUTOLINK_FMOD)
#	pragma comment(lib, "plugin_fmod.lib")
#endif
