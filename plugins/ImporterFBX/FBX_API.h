#pragma once

// define CUTE_FBX_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_FBX)
// build or use lib
#	define CUTE_FBX_API
#elif defined(CUTE_BUILD_FBX)
// build dynamic dll
#	define CUTE_FBX_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FBX_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FBX) && !defined(CUTE_UNAUTOLINK_FBX)
#	pragma comment(lib, "plugin_fbx.lib")
#endif
