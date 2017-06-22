// module Core
#pragma once
#include "Cute/API.h"

//////////////////////////////////////////////////////////////////////////
// 宏定义检测
//////////////////////////////////////////////////////////////////////////
///* ==== PLATFORM handles OS, operating environment, graphics API, and
//CPU. This macro will be phased out in favor of platform adaptation
//macros, policy decision macros, and top-level port definitions. ==== */
//#define PLATFORM(FEATURE) (defined CUTE_PLATFORM_##FEATURE )
//
///* ==== Platform adaptation macros: these describe properties of the target environment. ==== */
//
///* OS() - underlying operating system; only to be used for mandated low-level services like
//virtual memory, not to choose a GUI toolkit */
//#define OS(FEATURE) (defined CUTE_OS_##FEATURE)
//
///* CPU() - the target CPU architecture */
//#define CPU(FEATURE) (defined CUTE_CPU_##FEATURE)
//
///* HAVE() - specific system features (headers, functions or similar) that are present or not */
//#define HAVE(FEATURE) (defined HAVE_##FEATURE)
//
///* COMPILER() - the compiler being used to build the project */
//#define COMPILER(FEATURE) (defined CUTE_COMPILER_##FEATURE)
//
///* ==== Policy decision macros: these define policy choices for a particular port. ==== */
//
///* USE() - use a particular third-party library or optional OS service */
//#define USE(FEATURE) (defined USE_##FEATURE)
///* ENABLE() - turn on a specific feature of WebKit */
//#define ENABLE(FEATURE) (defined ENABLE_##FEATURE)
//
//#define ENDIAN(FEATURE) (CUTE_BYTE_ORDER == CUTE_##FEATURE##_ENDIAN)

//////////////////////////////////////////////////////////////////////////
// Platform Identification
//////////////////////////////////////////////////////////////////////////
#define CUTE_OS_ANDROID	      0x0001
#define CUTE_OS_LINUX         0x0002
#define CUTE_OS_FREE_BSD      0x0003
#define CUTE_OS_AIX           0x0004
#define CUTE_OS_HPUX          0x0005
#define CUTE_OS_TRU64         0x0006
#define CUTE_OS_MAC_OS_X      0x0007
#define CUTE_OS_NET_BSD       0x0008
#define CUTE_OS_OPEN_BSD      0x0009
#define CUTE_OS_IRIX          0x000a
#define CUTE_OS_SOLARIS       0x000b
#define CUTE_OS_QNX           0x000c
#define CUTE_OS_VXWORKS       0x000d
#define CUTE_OS_CYGWIN        0x000e
#define CUTE_OS_NACL	      0x000f
#define CUTE_OS_UNKNOWN_UNIX  0x00ff
#define CUTE_OS_WINDOWS_NT    0x1001
#define CUTE_OS_WINDOWS_RT    0x1002
#define CUTE_OS_WINDOWS_CE    0x1003
#define CUTE_OS_VMS           0x2001

#if defined(__ANDROID__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX  1
#	define CUTE_OS_FAMILY_LINUX 1
#	define CUTE_OS CUTE_OS_ANDROID
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS_FAMILY_BSD	1
#	define CUTE_OS CUTE_OS_FREE_BSD
#elif defined(_AIX) || defined(__TOS_AIX__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_AIX
#elif defined(hpux) || defined(_hpux) || defined(__hpux)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_HPUX
#elif defined(__digital__) || defined(__osf__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_TRU64
#elif defined(__NACL__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_NACL
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS_FAMILY_BSD	1
#	define CUTE_OS CUTE_OS_MAC_OS_X
#elif defined(__NetBSD__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS_FAMILY_BSD	1
#	define CUTE_OS CUTE_OS_NET_BSD
#elif defined(__OpenBSD__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS_FAMILY_BSD	1
#	define CUTE_OS CUTE_OS_OPEN_BSD
#elif defined(sgi) || defined(__sgi)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_IRIX
#elif defined(sun) || defined(__sun)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_SOLARIS
#elif defined(__QNX__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_QNX
#elif defined(__CYGWIN__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_CYGWIN
#elif defined(__VXWORKS__) || defined(__vxworks) || defined(CUTE_VXWORKS)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_VXWORKS
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__) || defined(EMSCRIPTEN)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_LINUX
#elif defined(unix) || defined(__unix) || defined(__unix__)
#	define CUTE_OS_FAMILY_POSIX 1
#	define CUTE_OS_FAMILY_UNIX	1
#	define CUTE_OS CUTE_OS_UNKNOWN_UNIX
#elif defined(_WIN32_WCE)
#	define CUTE_OS_FAMILY_WINDOWS 1
#	define CUTE_OS CUTE_OS_WINDOWS_CE
#elif defined(_WIN32) || defined(_WIN64)
#	define CUTE_OS_FAMILY_WINDOWS 1
#	define CUTE_OS CUTE_OS_WINDOWS_NT
#elif defined(__VMS)
#	define CUTE_OS_FAMILY_VMS 1
#	define CUTE_OS CUTE_OS_VMS
#endif

//////////////////////////////////////////////////////////////////////////
// Hardware Architecture
//////////////////////////////////////////////////////////////////////////
#define CUTE_ARCH_ALPHA   0x01
#define CUTE_ARCH_IA32    0x02
#define CUTE_ARCH_IA64    0x03
#define CUTE_ARCH_MIPS    0x04
#define CUTE_ARCH_HPPA    0x05
#define CUTE_ARCH_PPC     0x06
#define CUTE_ARCH_POWER   0x07
#define CUTE_ARCH_SPARC   0x08
#define CUTE_ARCH_AMD64   0x09
#define CUTE_ARCH_ARM     0x0a
#define CUTE_ARCH_ARM64   0x0b
#define CUTE_ARCH_M68K    0x0c
#define CUTE_ARCH_S390    0x0d
#define CUTE_ARCH_SH      0x0e
#define CUTE_ARCH_NIOS2   0x0f
#define CUTE_ARCH_AARCH64 0x10

#if defined(__ALPHA) || defined(__alpha) || defined(__alpha__) || defined(_M_ALPHA)
#	define CUTE_ARCH CUTE_ARCH_ALPHA
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(EMSCRIPTEN)
#	define CUTE_ARCH CUTE_ARCH_IA32
#elif defined(__x86_64__) || defined(_M_X64)
#	define CUTE_ARCH CUTE_ARCH_AMD64
#elif defined(__hppa) || defined(__hppa__)
#	define CUTE_ARCH CUTE_ARCH_HPPA
#elif defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || defined(_ARCH_PWR4) || defined(__THW_RS6000)
#	define CUTE_ARCH CUTE_ARCH_POWER
#elif defined(__sparc__) || defined(__sparc) || defined(sparc)
#	define CUTE_ARCH CUTE_ARCH_SPARC
#elif defined(__m68k__)
#	define CUTE_ARCH CUTE_ARCH_M68K
#elif defined(__s390__)
#	define CUTE_ARCH CUTE_ARCH_S390
#elif defined(__AARCH64EL__) || defined(__AARCH64EB__)
#	define CUTE_ARCH CUTE_ARCH_AARCH64
#elif defined(_IA64) || defined(__IA64__) || defined(__ia64__) || defined(__ia64) || defined(_M_IA64)
#	define CUTE_ARCH CUTE_ARCH_IA64
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__) || defined(_M_MRX000)
#	define CUTE_ARCH CUTE_ARCH_MIPS
#elif defined(__PPC) || defined(__POWERPC__) || defined(__powerpc) || defined(__PPC__) || defined(__powerpc__) || defined(__ppc__) || defined(__ppc) || defined(_ARCH_PPC) || defined(_M_PPC)
#	define CUTE_ARCH CUTE_ARCH_PPC
#elif defined(__arm__) || defined(__arm) || defined(ARM) || defined(_ARM_) || defined(__ARM__) || defined(_M_ARM)
#	define CUTE_ARCH CUTE_ARCH_ARM
#elif defined(__arm64__) || defined(__arm64) 
#	define CUTE_ARCH CUTE_ARCH_ARM64
#elif defined(__sh__) || defined(__sh) || defined(SHx) || defined(_SHX_)
#	define CUTE_ARCH CUTE_ARCH_SH
#elif defined (nios2) || defined(__nios2) || defined(__nios2__)
#	define CUTE_ARCH CUTE_ARCH_NIOS2
#endif

#if !defined(CUTE_ARCH)
#error "Unknown Hardware Architecture."
#endif

//////////////////////////////////////////////////////////////////////////
// BYTE_ORDER check
//////////////////////////////////////////////////////////////////////////
#define CUTE_ENDIAN_LITTLE	1234
#define CUTE_ENDIAN_BIG		4321
#define CUTE_ENDIAN_PDP		3412

#if (CUTE_ARCH == CUTE_ARCH_ALPHA) || (CUTE_ARCH == CUTE_ARCH_IA32)  || \
	(CUTE_ARCH == CUTE_ARCH_AMD64) || (CUTE_ARCH == CUTE_ARCH_AARCH64)
#	define CUTE_ENDIAN CUTE_ENDIAN_LITTLE

#elif (CUTE_ARCH == CUTE_ARCH_HPPA) || (CUTE_ARCH == CUTE_ARCH_POWER) || \
	(CUTE_ARCH == CUTE_ARCH_SPARC) || (CUTE_ARCH == CUTE_ARCH_M68K) ||	 \
	(CUTE_ARCH == CUTE_ARCH_S390)
#	define CUTE_ENDIAN CUTE_ENDIAN_BIG

#elif (CUTE_ARCH == CUTE_ARCH_MIPS)
#	if defined(__MIPSEB__) || defined(_MIPSEB) || defined(__MIPSEB)
#	define CUTE_ENDIAN CUTE_ENDIAN_BIG
#	elif defined(__MIPSEL__) || defined(_MIPSEL) || defined(__MIPSEL)
#	define CUTE_ENDIAN CUTE_ENDIAN_LITTLE
#	else
#	error "MIPS but neither MIPSEL nor MIPSEB?"
#	endif

#elif (CUTE_ARCH == CUTE_ARCH_ARM) || (CUTE_ARCH == CUTE_ARCH_ARM64)
#	if defined(__ARMEB__)
#	define CUTE_ENDIAN CUTE_ENDIAN_BIG
#	else
#	define CUTE_ENDIAN CUTE_ENDIAN_LITTLE
#	endif

#elif (CUTE_ARCH == CUTE_ARCH_IA64)
#	if defined(hpux) || defined(_hpux)
#	define CUTE_ENDIAN CUTE_ENDIAN_BIG
#	else
#	define CUTE_ENDIAN CUTE_ENDIAN_LITTLE
#	endif

#endif

// check __BYTE_ORDER__
#ifndef CUTE_BYTE_ORDER
#	ifdef __BYTE_ORDER__
#		if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#		define CUTE_ENDIAN CUTE_ENDIAN_BIG
#		else
#		define CUTE_ENDIAN CUTE_ENDIAN_LITTLE
#		endif
#	elif defined(CUTE_OS_FAMILY_WINDOWS)
#	define CUTE_ENDIAN CUTE_ENDIAN_LITTLE
#	elif (CUTE_OS == CUTE_OS_HPUX)
#	define CUTE_ENDIAN CUTE_ENDIAN_BIG
#	else
#	error "unknown endian!!"
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// compiler check
//////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#	define CUTE_COMPILER_MSVC
#elif defined(__clang__)
#	define CUTE_COMPILER_CLANG
#elif defined (__GNUC__)
#	define CUTE_COMPILER_GCC
#elif defined (__MINGW32__) || defined (__MINGW64__)
#	define CUTE_COMPILER_MINGW
#elif defined (__INTEL_COMPILER) || defined(__ICC) || defined(__ECC) || defined(__ICL)
#	define CUTE_COMPILER_INTEL
#elif defined (__SUNPRO_CC)
#	define CUTE_COMPILER_SUN
#elif defined (__MWERKS__) || defined(__CWCC__)
#	define CUTE_COMPILER_CODEWARRIOR
#elif defined (__sgi) || defined(sgi)
#	define CUTE_COMPILER_SGI
#elif defined (__HP_aCC)
#	define CUTE_COMPILER_HP_ACC
#elif defined (__BORLANDC__) || defined(__CODEGEARC__)
#	define CUTE_COMPILER_CBUILDER
#elif defined (__DMC__)
#	define CUTE_COMPILER_DMARS
#elif defined (__DECCXX)
#	define CUTE_COMPILER_COMPAC
#elif (defined (__xlc__) || defined (__xlC__)) && defined(__IBMCPP__)
#	define CUTE_COMPILER_IBM_XLC // IBM XL C++
#elif defined (__IBMCPP__) && defined(__COMPILER_VER__)
#	define CUTE_COMPILER_IBM_XLC_ZOS // IBM z/OS C++
#endif

//////////////////////////////////////////////////////////////////////////
// check 32 bit or 64 bit
//////////////////////////////////////////////////////////////////////////
#if defined(__SUNPRO_CC)
#	if defined(__sparcv9)
#	define CUTE_BIT64
#	else
#	define CUTE_BIT32
#	endif
#elif defined(__sgi)

#	if (_MIPS_SZLONG == 64)
#	define CUTE_BIT64
#	else
#	define CUTE_BIT32
#	endif

#elif defined(__DECCXX)

#	if defined(__VMS) && defined(__32BITS)
#	define CUTE_BIT32
#	else
#	define CUTE_BIT64
#	endif

#elif defined(_DIAB_TOOL)

#	define CUTE_BIT64

#else
// _MSC_VER __GNUC__ __clang__ __HP_aCC  __IBMCPP__
#	if defined(_WIN64) || defined(__LP64__) || defined(__64BIT__)
#	define CUTE_BIT64
#	else
#	define CUTE_BIT32
#	endif

#endif

//////////////////////////////////////////////////////////////////////////
// string编码类型,window默认ascii, linux utf8
//////////////////////////////////////////////////////////////////////////
//#define CUTE_STRING_ASCII	0
//#define CUTE_STRING_WIDE	1
//#define CUTE_STRING_UTF8	2
//
//#ifndef CUTE_STRING_CODE
//#define CUTE_STRING_CODE	CUTE_STRING_UTF8
//#endif

//////////////////////////////////////////////////////////////////////////
// 不同平台常见设置
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#	define WIN32_LEAN_AND_MEAN
#   define NOMINMAX

// Verify that we're built with the multithreaded versions of the runtime libraries
#if !defined(_MT)
#	error Must compile with /MD, /MDd, /MT or /MTd
#endif

// Check debug/release settings consistency
#if defined(NDEBUG) && defined(_DEBUG)
#	error Inconsistent build settings (check for /MD[d])
#endif

// _CRT_SECURE_NO_DEPRECATE
#if !defined(_CRT_SECURE_NO_DEPRECATE) && !defined(CUTE_MSVC_SECURE_WARNINGS)
#	define _CRT_SECURE_NO_DEPRECATE
#endif

#if !defined(_SCL_SECURE_NO_WARNINGS)
#	define _SCL_SECURE_NO_WARNINGS
#endif

#if defined(_MSC_VER)
// Turn off some annoying warnings
// see <http://connect.microsoft.com/VisualStudio/feedback/details/733720/inheriting-from-std-fstream-produces-c4250-warning>
#	pragma warning(disable:4250)	// VC++ 11.0: inheriting from std stream classes produces C4250 warning;
#	pragma warning(disable:4018)	// signed/unsigned comparison
#	pragma warning(disable:4251)	// ... needs to have dll-interface warning
#	pragma warning(disable:4275)	// non dll-interface class 'std::exception' used as base for dll-interface class 'Poco::Exception'
#	pragma warning(disable:4344)	// behavior change: use of explicit template arguments results in call to '...' but '...' is a better match
#	pragma warning(disable:4351)	// new behavior: elements of array '...' will be default initialized
#	pragma warning(disable:4355)	// 'this' : used in base member initializer list
#	pragma warning(disable:4675)	// resolved overload was found by argument-dependent lookup
#	pragma warning(disable:4996)	// VC++ 8.0 deprecation warnings
#	pragma warning(disable:4819)	// utf8 coding warning
#endif

#if defined(__INTEL_COMPILER)
#	pragma warning(disable:1738) // base class dllexport/dllimport specification differs from that of the derived class
#	pragma warning(disable:1478) // function ... was declared "deprecated"
#	pragma warning(disable:1744) // field of class type without a DLL interface used in a class with a DLL interface
#endif

// define _MSC_VER  version
#	if (_MSC_VER >= 1300) && (_MSC_VER < 1400)   // Visual Studio 2003, MSVC++ 7.1
#	define CUTE_MSVS_VERSION 2003
#	define CUTE_MSVC_VERSION 71
#	elif (_MSC_VER >= 1400) && (_MSC_VER < 1500) // Visual Studio 2005, MSVC++ 8.0
#	define CUTE_MSVS_VERSION 2005
#	define CUTE_MSVC_VERSION 80
#	elif (_MSC_VER >= 1500) && (_MSC_VER < 1600) // Visual Studio 2008, MSVC++ 9.0
#	define CUTE_MSVS_VERSION 2008
#	define CUTE_MSVC_VERSION 90
#	elif (_MSC_VER >= 1600) && (_MSC_VER < 1700) // Visual Studio 2010, MSVC++ 10.0
#	define CUTE_MSVS_VERSION 2010
#	define CUTE_MSVC_VERSION 100
#	elif (_MSC_VER >= 1700) && (_MSC_VER < 1800) // Visual Studio 2012, MSVC++ 11.0
#	define CUTE_MSVS_VERSION 2012
#	define CUTE_MSVC_VERSION 110
#	elif (_MSC_VER >= 1800) && (_MSC_VER < 1900) // Visual Studio 2013, MSVC++ 12.0
#	define CUTE_MSVS_VERSION 2013
#	define CUTE_MSVC_VERSION 120
#	elif (_MSC_VER >= 1900) && (_MSC_VER < 2000) // Visual Studio 2015, MSVC++ 14.0
#	define CUTE_MSVS_VERSION 2015
#	define CUTE_MSVC_VERSION 140
#	else
#	define CUTE_MSVS_VERSION 2017
#	define CUTE_MSVC_VERSION 150
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// some specil function
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#if (_MSC_VER < 1900) && !defined(snprintf)
#	define snprintf				sprintf_s
#endif
#	define strcasecmp			stricmp		// 忽略字符大小写，strncasecmp比较前n个字符
#	define strncasecmp			strnicmp 

#	define CUTE_CLOSE_SOCKET(s)	::closesocket(s)
#	define CUTE_CLOSE_HANDLE(h)	::CloseHandle(h)
#	define CUTE_LAST_ERROR()	::GetLastError()
#else
#	define CUTE_CLOSE_SOCKET(s)	::close(s)
#	define CUTE_CLOSE_HANDLE(s)	::close(s)
#	define CUTE_LAST_ERROR()	errno
#endif

//////////////////////////////////////////////////////////////////////////
// some specil macro
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#	define SHUT_RD				SD_RECEIVE
#	define SHUT_WR				SD_SEND
#	define SHUT_RDWR			SD_BOTH

/* Additional signals supported by uv_signal and or uv_kill. The CRT defines
* the following signals already:
*
*   #define SIGINT           2
*   #define SIGILL           4
*   #define SIGABRT_COMPAT   6
*   #define SIGFPE           8
*   #define SIGSEGV         11
*   #define SIGTERM         15
*   #define SIGBREAK        21
*   #define SIGABRT         22
*
* The additional signals have values that are common on other Unix
* variants (Linux and Darwin)
*/
#define SIGHUP                1
#define SIGKILL               9
#define SIGWINCH             28

/* The CRT defines SIGABRT_COMPAT as 6, which equals SIGABRT on many */
/* unix-like platforms. However MinGW doesn't define it, so we do. */
#ifndef SIGABRT_COMPAT
# define SIGABRT_COMPAT       6
#endif

#ifndef S_IFLNK
# define S_IFLNK 0xA000
#endif

#else

#   define MAX_PATH				MAXPATHLEN
#	define INVALID_HANDLE_VALUE 0
#	define INVALID_SOCKET		0

#endif

// remove??
#if defined(CUTE_OS_FAMILY_UNIX)
//
// Note: pthread_cond_timedwait() with CLOCK_MONOTONIC is supported
// on Linux and QNX, as well as on Android >= 5.0. On Android < 5.0,
// HAVE_PTHREAD_COND_TIMEDWAIT_MONOTONIC is defined to indicate 
// availability of non-standard pthread_cond_timedwait_monotonic().
//
#	if (defined(__linux__) || defined(__QNX__)) && !(defined(__ANDROID__) && defined(HAVE_PTHREAD_COND_TIMEDWAIT_MONOTONIC))
#	define CUTE_HAVE_MONOTONIC_PTHREAD_COND_TIMEDWAIT 1
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// lib suffix
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#	define CUTE_DLL_SUFFIX	".dll"
#elif defined(CUTE_OS_MAC_OS_X)
#	define CUTE_DLL_SUFFIX	".dylib"
#else
#	define CUTE_DLL_SUFFIX	".so"
#endif

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
#ifdef _MSC_VER
#	define CUTE_EXPORT __declspec(dllexport)
#	define CUTE_IMPORT __declspec(dllimport)
#else
#	define CUTE_EXPORT
#	define CUTE_IMPORT
#endif

//////////////////////////////////////////////////////////////////////////
// 其他一些特性检测
//////////////////////////////////////////////////////////////////////////
#if (__cplusplus > 199711L) || (defined(__STDCXX_VERSION__) && (__STDCXX_VERSION__ >= 201001L))
#	define CUTE_CPP0X
#endif

// Enable C++11 support for VS 2014 and newer
#if (_MSC_VER >= 1900) ||  (__cplusplus >= 201103L)
#define CUTE_CPP11
#endif

#ifndef OVERRIDE
# if defined(CUTE_CPP11)
#   define OVERRIDE override
# else
#   define OVERRIDE
# endif
#endif

// 变长模板
#ifdef CUTE_CPP11
#define CUTE_VARIADIC
#endif

#ifdef CUTE_CPP11
#define CUTE_ENUM_TYPE(type) : type
#else
#define CUTE_ENUM_TYPE(type)
#endif

// __PRETTY_FUNCTION__:会带有函数参数
#if defined(_MSC_VER) && !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// FUNC
#ifndef __FUNC__
#define __FUNC__ __FUNCTION__
#endif

//#ifdef __GNUC__
//#define CUTE_UNUSED __attribute__((unused))
//#else
//#define CUTE_UNUSED
//#endif // __GNUC__

#if defined(CUTE_OS_FAMILY_WINDOWS)
#define CUTE_NEWLINE_CHARS "\r\n"
#else
#define CUTE_NEWLINE_CHARS "\n"
#endif

#if defined(_WIN32)
#define CUTE_PATH_SEP	'\\'
#else
#define CUTE_PATH_SEP	'/'
#endif

#if defined(_MSC_VER)
#	define I64_FMT "I64"
#elif defined(__APPLE__)
#	define I64_FMT "q"
#else
#	define I64_FMT "ll"
#endif

//////////////////////////////////////////////////////////////////////////
// Macro
//////////////////////////////////////////////////////////////////////////
#define CUTE_UNUSED(x)				(void)(x)
#define CUTE_DELETE(ptr)			if(ptr) { delete ptr; ptr = NULL; }
#define CUTE_FOURCC(a, b, c, d)		(((uint32_t)a) | (((uint32_t)b)<<8) | (((uint32_t)c)<<16) | (((uint32_t)d) << 24))
#define CUTE_ARRAY_SIZE(a)			(sizeof(a)/sizeof(a[0]))

#define CUTE_FLOAT_SMALL            1.0e-37f
#define CUTE_TOLERANCE              2e-37f
#define CUTE_EPSILON                0.000001f
#define CUTE_E                      2.71828182845904523536f
#define CUTE_LOG10E                 0.4342944819032518f
#define CUTE_LOG2E                  1.442695040888963387f
#define CUTE_PI                     3.14159265358979323846f
#define CUTE_PIOVER2                1.57079632679489661923f
#define CUTE_PIOVER4                0.785398163397448309616f
#define CUTE_PIX2                   6.28318530717958647693f
#define CUTE_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define CUTE_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define CUTE_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))

#define NPOS						((unsigned int)-1)

#define ENGINE_NAME					"CuteEngine"

#define CUTE_NS_BEGIN namespace Cute {
#define CUTE_NS_END	}
#define CUTE_NS_USING using namespace Cute;
