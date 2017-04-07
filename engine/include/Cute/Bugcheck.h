#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// This class provides some static methods that are
/// used by the
/// cute_assert_dbg(), cute_assert(), cute_check_ptr(), 
/// cute_bugcheck() and cute_unexpected() macros. 
/// You should not invoke these methods
/// directly. Use the macros instead, as they
/// automatically provide useful context information.
class CUTE_CORE_API Bugcheck
{
public:
	static void assertion(const char* cond, const char* file, int line, const char* text = 0);
	/// An assertion failed. Break into the debugger, if
	/// possible, then throw an AssertionViolationException.

	static void nullPointer(const char* ptr, const char* file, int line);
	/// An null pointer was encountered. Break into the debugger, if
	/// possible, then throw an NullPointerException.

	static void bugcheck(const char* file, int line);
	/// An internal error was encountered. Break into the debugger, if
	/// possible, then throw an BugcheckException.

	static void bugcheck(const char* msg, const char* file, int line);
	/// An internal error was encountered. Break into the debugger, if
	/// possible, then throw an BugcheckException.

	static void unexpected(const char* file, int line);
	/// An exception was caught in a destructor. Break into debugger,
	/// if possible and report exception. Must only be called from
	/// within a catch () block as it rethrows the exception to
	/// determine its class.

	static void debugger(const char* file, int line);
	/// An internal error was encountered. Break into the debugger, if
	/// possible.

	static void debugger(const char* msg, const char* file, int line);
	/// An internal error was encountered. Break into the debugger, if
	/// possible.

protected:
	static std::string what(const char* msg, const char* file, int line, const char* text = 0);
};

CUTE_NS_END

  //
  // useful macros (these automatically supply line number and file name)
  //
#if defined(__KLOCWORK__) || defined(__clang_analyzer__)
  // Short-circuit these macros when under static analysis.
  // Ideally, static analysis tools should understand and reason correctly about
  // noreturn methods such as Bugcheck::bugcheck(). In practice, they don't.
  // Help them by turning these macros into std::abort() as described here:
  // https://developer.klocwork.com/documentation/en/insight/10-1/tuning-cc-analysis#Usingthe__KLOCWORK__macro

#include <cstdlib> // for abort
#define cute_assert_dbg(cond)           do { if (!(cond)) std::abort(); } while (0)
#define cute_assert_msg_dbg(cond, text) do { if (!(cond)) std::abort(); } while (0)
#define cute_assert(cond)               do { if (!(cond)) std::abort(); } while (0)
#define cute_assert_msg(cond, text)     do { if (!(cond)) std::abort(); } while (0)
#define cute_check_ptr(ptr)             do { if (!(ptr)) std::abort(); } while (0)
#define cute_bugcheck()                 do { std::abort(); } while (0)
#define cute_bugcheck_msg(msg)          do { std::abort(); } while (0)


#else // defined(__KLOCWORK__) || defined(__clang_analyzer__)


#if defined(_DEBUG)
#define cute_assert_dbg(cond) \
		if (!(cond)) Cute::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0

#define cute_assert_msg_dbg(cond, text) \
		if (!(cond)) Cute::Bugcheck::assertion(#cond, __FILE__, __LINE__, text); else (void) 0
#else
#define cute_assert_msg_dbg(cond, text)
#define cute_assert_dbg(cond)
#endif


#define cute_assert(cond) \
	if (!(cond)) Cute::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0


#define cute_assert_msg(cond, text) \
	if (!(cond)) Cute::Bugcheck::assertion(#cond, __FILE__, __LINE__, text); else (void) 0


#define cute_check_ptr(ptr) \
	if (!(ptr)) Cute::Bugcheck::nullPointer(#ptr, __FILE__, __LINE__); else (void) 0


#define cute_bugcheck() \
	Cute::Bugcheck::bugcheck(__FILE__, __LINE__)


#define cute_bugcheck_msg(msg) \
	Cute::Bugcheck::bugcheck(msg, __FILE__, __LINE__)


#endif // defined(__KLOCWORK__) || defined(__clang_analyzer__)


#define cute_unexpected() \
	Cute::Bugcheck::unexpected(__FILE__, __LINE__);


#define cute_debugger() \
	Cute::Bugcheck::debugger(__FILE__, __LINE__)


#define cute_debugger_msg(msg) \
	Cute::Bugcheck::debugger(msg, __FILE__, __LINE__)


#if defined(_DEBUG)
#	define cute_stdout_dbg(outstr) \
	std::cout << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else
#	define cute_stdout_dbg(outstr)
#endif

#if defined(_DEBUG)
#	define cute_stderr_dbg(outstr) \
		std::cerr << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else
#	define cute_stderr_dbg(outstr)
#endif

  //
  // cute_static_assert
  //
  // The following was ported from <boost/static_assert.hpp>
  //
template <bool x>
struct CUTE_STATIC_ASSERTION_FAILURE;

template <>
struct CUTE_STATIC_ASSERTION_FAILURE<true>
{
	enum
	{
		value = 1
	};
};

template <int x>
struct cute_static_assert_test
{
};

#if defined(__GNUC__) && (__GNUC__ == 3) && ((__GNUC_MINOR__ == 3) || (__GNUC_MINOR__ == 4))
#define cute_static_assert(B) \
	typedef char CUTE_JOIN(cute_static_assert_typedef_, __LINE__) \
        [CUTE_STATIC_ASSERTION_FAILURE<(bool) (B)>::value]
#else
#define cute_static_assert(B) \
	typedef cute_static_assert_test<sizeof(CUTE_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
		CUTE_JOIN(cute_static_assert_typedef_, __LINE__)
#endif
