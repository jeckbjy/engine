#pragma once

// define CUTE_OAI_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_ASSIMP)
// build or use lib
#	define CUTE_OAI_API
#elif defined(CUTE_BUILD_ASSIMP)
// build dynamic dll
#	define CUTE_OAI_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_OAI_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_ASSIMP) && !defined(CUTE_UNAUTOLINK_ASSIMP)
#	pragma comment(lib, "plugin_assimp.lib")
#endif
