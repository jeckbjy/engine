#pragma once

// define CUTE_ODE_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_ODE)
// build or use lib
#	define CUTE_ODE_API
#elif defined(CUTE_BUILD_ODE)
// build dynamic dll
#	define CUTE_ODE_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_ODE_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_ODE) && !defined(CUTE_UNAUTOLINK_ODE)
#	pragma comment(lib, "plugin_ode.lib")
#endif
