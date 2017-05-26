#pragma once

// define CUTE_WAVE_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_WAVE)
// build or use lib
#	define CUTE_WAVE_API
#elif defined(CUTE_BUILD_WAVE)
// build dynamic dll
#	define CUTE_WAVE_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_WAVE_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_WAVE) && !defined(CUTE_UNAUTOLINK_WAVE)
#	pragma comment(lib, "plugin_wave.lib")
#endif
