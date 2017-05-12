#pragma once

// define CUTE_ODE_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
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
