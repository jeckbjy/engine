//! API
#pragma once

//////////////////////////////////////////////////////////////////////////
// 导出lib
//////////////////////////////////////////////////////////////////////////
/*
 只有microsoft的编译器需要
 用途：
 1：编译器可以优化函数调用
 2：静态成员变量和成员函数必须使用
 3：全局变量的导出
 */
#if defined(_MSC_VER)
#	define CUTE_EXPORT __declspec(dllexport)
#	define CUTE_IMPORT __declspec(dllimport)
#else
#	define CUTE_EXPORT
#	define CUTE_IMPORT
#endif

// define CUTE_CORE_API
// 默认使用DLL，如果使用LIB需要单独定义
#if defined(CUTE_USE_LIB_CORE)
// build or use lib
#   define CUTE_CORE_API
#elif defined(CUTE_BUILD_CORE)
// build dynamic dll
#   define CUTE_CORE_API CUTE_EXPORT
#else
// use static lib
#   define CUTE_CORE_API CUTE_IMPORT
#endif

#define CUTE_NS_BEGIN   namespace Cute {
#define CUTE_NS_END     }
#define CUTE_NS_USING   using namespace Cute;

#define ENGINE_NAME		"CuteEngine"

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_CORE) && !defined(CUTE_UNAUTOLINK_CORE)
#   pragma comment(lib, "engine.lib")
#endif

#if defined(_MSC_VER)
#   pragma comment(lib, "ws2_32.lib")
#endif
