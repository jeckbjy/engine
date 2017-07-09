//! API
#pragma once

/* This file has some checks to see whether the compiler supports various C++11/14 features,
 When these aren't available, the code defines a few workarounds, so that we can still use
 some of the newer language features like nullptr and noexcept, even on old compilers.
 */

//==============================================================================
// GCC
#if (__cplusplus >= 201103L || defined (__GXX_EXPERIMENTAL_CXX0X__)) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 405
#define CUTE_COMPILER_SUPPORTS_NOEXCEPT 1
#define CUTE_COMPILER_SUPPORTS_INITIALIZER_LISTS    1
#define CUTE_COMPILER_SUPPORTS_VARIADIC_TEMPLATES   1

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined (CUTE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL)
#   define CUTE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL   1
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined (CUTE_DELETED_FUNCTION)
#   define CUTE_DELETED_FUNCTION = delete
#endif

#if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && ! defined (CUTE_COMPILER_SUPPORTS_LAMBDAS)
#   define CUTE_COMPILER_SUPPORTS_LAMBDAS          1
#   define CUTE_STDLIB_HAS_STD_FUNCTION_SUPPORT    1
#endif

#ifndef CUTE_EXCEPTIONS_DISABLED
#   if ! __EXCEPTIONS
#   define CUTE_EXCEPTIONS_DISABLED 1
#   endif
#endif
#endif


//==============================================================================
// Clang
#if __clang__ && defined (__has_feature)

#if __has_feature (cxx_noexcept)
#define CUTE_COMPILER_SUPPORTS_NOEXCEPT 1
#endif

#if __has_feature (cxx_deleted_functions)
#define CUTE_DELETED_FUNCTION = delete
#endif

#if __has_feature (cxx_lambdas)
#define CUTE_COMPILER_SUPPORTS_LAMBDAS 1
#endif

#if (defined (_LIBCPP_VERSION) || !defined(__APPLE__))
#define CUTE_STDLIB_HAS_STD_FUNCTION_SUPPORT 1
#endif

#if __has_feature (cxx_generalized_initializers) && (defined (_LIBCPP_VERSION) || !defined(__APPLE__))
#define CUTE_COMPILER_SUPPORTS_INITIALIZER_LISTS 1
#endif

#if __has_feature (cxx_variadic_templates)
#define CUTE_COMPILER_SUPPORTS_VARIADIC_TEMPLATES 1
#endif

#if __has_feature (cxx_override_control) && (! defined (CUTE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL))
#define CUTE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL 1
#endif

#ifndef CUTE_COMPILER_SUPPORTS_ARC
#define CUTE_COMPILER_SUPPORTS_ARC 1
#endif

#ifndef CUTE_EXCEPTIONS_DISABLED
#  if ! __has_feature (cxx_exceptions)
#  define CUTE_EXCEPTIONS_DISABLED 1
#  endif
#endif

#endif

//==============================================================================
// MSVC
#ifdef _MSC_VER

#if _MSC_VER >= 1700
#define CUTE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL   1
#define CUTE_COMPILER_SUPPORTS_LAMBDAS              1
#endif

#if _MSC_VER >= 1800
#define CUTE_COMPILER_SUPPORTS_INITIALIZER_LISTS    1
#define CUTE_COMPILER_SUPPORTS_VARIADIC_TEMPLATES   1
#define CUTE_DELETED_FUNCTION = delete
#define CUTE_STDLIB_HAS_STD_FUNCTION_SUPPORT        1
#endif

#if _MSC_VER >= 1900
#define CUTE_COMPILER_SUPPORTS_NOEXCEPT             1
#endif

#ifndef CUTE_EXCEPTIONS_DISABLED
#  if ! _CPPUNWIND
#  define CUTE_EXCEPTIONS_DISABLED 1
#  endif
#endif

#endif

//==============================================================================
//check supports
#if !defined(CUTE_COMPILER_SUPPORTS_NOEXCEPT)
#   ifdef noexcept
#   undef noexcept
#   endif

#   define noexcept throw()

#   if defined (_MSC_VER) && _MSC_VER > 1600
#   define _ALLOW_KEYWORD_MACROS 1 // (to stop VC2012 complaining)
#   endif
#endif
