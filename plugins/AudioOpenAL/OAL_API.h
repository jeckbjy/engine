#pragma once

// define CUTE_OPENAL_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_OPENAL)
// build or use lib
#	define CUTE_OAL_API
#elif defined(CUTE_BUILD_OPENAL)
// build dynamic dll
#	define CUTE_OAL_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_OAL_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_OPENAL) && !defined(CUTE_UNAUTOLINK_OPENAL)
#	pragma comment(lib, "plugin_openal.lib")
#endif

#include "Cute/Foundation.h"
#include "AL/alc.h"
#include "AL/al.h"
