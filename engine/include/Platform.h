#pragma once
//////////////////////////////////////////////////////////////////////////
// 不同平台定义
// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
//1:window：都会定义_WIN32,_WIN64只有在64位系统下才定义，WIN32是window.h下定义的
//2:__unix,__unix__:绝大部分平台都会定义__unix__,但apple(定义了__APPLE__)和Solaris没有定义(仅定义了__unix)
//_MSC_VER:版本说明：vs2010:_MSC_VER=1600,
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#	define CU_OS_WIN

// 检测winnt or winrt(metro风格)
#	ifndef WINAPI_FAMILY
#	define CU_OS_WINNT
#	else
#	define CU_OS_WINRT
#	endif

// 检测桌面or手机
#if defined(WINAPI_FAMILY)
#		if WINAPI_FAMILY == WINAPI_FAMILY_APP
#		define CU_OS_WINRT_APP
#		elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#		define CU_OS_WINRT_PHONE
#		endif
#	endif

#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))

#	define CU_OS_POSIX
#	define CU_OS_UNIX
// linux
#if defined(__linux__) || defined(__linux)
#	define CU_OS_LINUX
#endif

// bsd
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#	include <sys/param.h>
#if defined(BSD)
/* BSD (DragonFly BSD, FreeBSD, OpenBSD, NetBSD). ----------- */
#	define CU_OS_BSD
#endif
#endif

#if defined(__APPLE__) && defined(__MACH__)
#	define CU_OS_APPLE
/* Apple OSX and iOS (Darwin). ------------------------------ */
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#	define CU_OS_IPHONE
#	elif TARGET_OS_MAC
#	define CU_OS_MAC
#	endif
#endif

#if defined(__ANDROID__)
#	define CU_OS_ANDROID
#endif

// 黑莓系统,属于unix系统分支
#if defined(__QNX__)
#	define CU_OS_QNX
#endif

#endif

//////////////////////////////////////////////////////////////////////////
// 硬件架构检测
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 大小端, 有问题
//////////////////////////////////////////////////////////////////////////
#define CU_ENDIAN_LITTLE		1
#define CU_ENDIAN_BIG			2
#define CU_ENDIAN_PDP			3

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#	define CU_ENDIAN CU_ENDIAN_LITTLE
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#	define CU_ENDIAN CU_ENDIAN_BIG
#endif
#elif defined(__LITTLE_ENDIAN__)
#	define CU_ENDIAN CU_ENDIAN_LITTLE
#elif defined(__BIG_ENDIAN__)
#	define CU_ENDIAN CU_ENDIAN_BIG
#else
#define __ENDIAN_ORDER_LITTLE	0x41424344UL
#define __ENDIAN_ORDER_BIG		0x44434241UL
#define __ENDIAN_ORDER_PDP		0x42414443UL
#define __ENDIAN_ORDER_WORD		('ABCD')
#if __ENDIAN_ORDER_WORD == __ENDIAN_ORDER_LITTLE
#	define CU_ENDIAN CU_ENDIAN_LITTLE
#elif __ENDIAN_ORDER_WORD == __ENDIAN_ORDER_BIG
#	define CU_ENDIAN CU_ENDIAN_BIG
#elif __ENDIAN_ORDER_WORD == __ENDIAN_ORDER_PDP
#	define CU_ENDIAN CU_ENDIAN_PDP
#endif
#undef __ENDIAN_ORDER_LITTLE
#undef __ENDIAN_ORDER_BIG
#undef __ENDIAN_ORDER_PDP
#undef __ENDIAN_ORDER_WORD
#endif

//////////////////////////////////////////////////////////////////////////
// c++11 相关特性检测
//////////////////////////////////////////////////////////////////////////
#if (__cplusplus > 199711L) || (defined(__STDCXX_VERSION__) && (__STDCXX_VERSION__ >= 201001L))
#	define CU_CPP0X
#endif

// msvc:1600 = vs2010
#if __cplusplus >= 201103L || _MSC_VER > 1600
#	define CU_CPP11
#endif

// 实现了部分
#if defined(CU_CPP0X) || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(__GXX_EXPERIMENTAL_CPP0X__)
#	define CU_CPP0X_PARTIAL
#endif

// 右值引用
#ifdef CU_CPP11
#	define CU_HAS_RVALUE
#endif

// 
#ifdef CU_CPP0X
#	define CU_NOEXCEPT noexcept
#	define CU_ENUM(type) :type

#else
#	define CU_NOEXCEPT 
#	define CU_ENUM(type)
#endif

//////////////////////////////////////////////////////////////////////////
// 禁止警告
//////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif
#   pragma warning (disable : 4786)
#   pragma warning (disable : 4503)
#   pragma warning (disable : 4251)
#   pragma warning (disable : 4275)
#   pragma warning (disable : 4290)
#   pragma warning (disable : 4661)
#   pragma warning (disable : 4996)
#   pragma warning (disable : 201)
#   pragma warning (disable : 4100)
//#   pragma warning (disable : 4345)
#	pragma warning (disable : 4244)
#endif

// __PRETTY_FUNCTION__:会带有函数参数
#if defined(_MSC_VER) && !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// 自动执行函数
#if defined(_MSC_VER)
// 不能随意加上下面这句，vs2013会导致莫名的宕机
//#pragma section(".CRT$XCU", read)
#define CU_C_CTOR(fn)                                               \
  static void __cdecl fn(void);                                     \
  __declspec(dllexport, allocate(".CRT$XCU"))                       \
      void (__cdecl*fn ## _)(void) = fn;                            \
  static void __cdecl fn(void)
#else
#define CU_C_CTOR(fn)                                               \
  static void fn(void) __attribute__((constructor));                \
  static void fn(void)
#endif

/*
只有microsoft的编译器需要
用途：
1：编译器可以优化函数调用
2：静态成员变量和成员函数必须使用
3：全局变量的导出
*/
#ifdef _MSC_VER
#	define CU_DECL_EXPORT __declspec(dllexport)
#	define CU_DECL_IMPORT __declspec(dllimport)
#else
#	define CU_DECL_EXPORT
#	define CU_DECL_IMPORT
#endif

// 链接方式，dll or lib，默认使用dll方式
#if defined(CU_BUILD_LIB) || defined(CU_USE_LIB)
#	define CU_API
#elif defined(CU_BUILD_DLL)
#	define CU_API CU_DECL_EXPORT
#else
#	define CU_API CU_DECL_IMPORT
#endif

#define CU_NS_BEGIN namespace cute {
#define CU_NS_END	}
#define CU_NS_USING using namespace cute
