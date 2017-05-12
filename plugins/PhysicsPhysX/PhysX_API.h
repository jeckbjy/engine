#pragma once

// define CUTE_PHYSX_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_PHYSX)
// build or use lib
#	define CUTE_PHYSX_API
#elif defined(CUTE_BUILD_PHYSX)
// build dynamic dll
#	define CUTE_PHYSX_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_PHYSX_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_PHYSX) && !defined(CUTE_UNAUTOLINK_PHYSX)
#	pragma comment(lib, "plugin_physx.lib")
#endif
