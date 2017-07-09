//! API
#pragma once
#include "Cute/Platform.h"
#include "Cute/CompilerConfig.h"
#include "Cute/Types.h"
#include "Cute/Strings.h"
#include "Cute/API.h"

#ifdef _WIN32
#   include <windows.h>
#	include <WinSock2.h>
#	include <WS2tcpip.h>
#	include <mswsock.h>
#	include <io.h>
#	include <tchar.h>
#	include <intrin.h>

#else
#	include <stdio.h>
#	include <unistd.h>
#	include <fcntl.h>
#	include <errno.h>
#	include <utime.h>

#	include <sys/stat.h>
#	include <sys/types.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#   include <sys/param.h>

#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <netdb.h>
#	include <stdarg.h>
// 多线程
#	include<pthread.h>

#if defined(__MACH__)
#	include <mach/mach.h>
#	include <mach/clock.h>
#	include <mach/mach_time.h>
#endif

#endif

CUTE_NS_BEGIN

#ifndef TRUE
#	define TRUE		1
#	define FALSE	0
#endif

#ifdef _WIN32
typedef HANDLE		handle_t;
typedef HMODULE		module_t;
typedef DWORD		error_t;
typedef DWORD		pid_t;
typedef SOCKET		socket_t;
typedef int			socklen_t;
typedef int			ioctl_req_t;
#else
typedef int			handle_t;
typedef void*		module_t;
typedef int			error_t;
typedef int			socket_t;
//typedef int			socklen_t;
#	if defined(CUTE_OS_FAMILY_BSD)
typedef unsigned long	ioctl_req_t;
#	else
typedef int			ioctl_req_t;
#	endif
#endif

#if defined(CUTE_OS_FAMILY_WINDOWS)

// 自动关闭
class HandleScope
{
public:
	HandleScope(HANDLE h) :m_handle(h){}
	~HandleScope()
	{
		if (m_handle != INVALID_HANDLE_VALUE)
			::CloseHandle(m_handle);
	}
private:
	HANDLE m_handle;
};

// Local编码与System编码相互转化
void CUTE_CORE_API ConvertToSystem(const String& src, TString& dst);
void CUTE_CORE_API ConvertToLocal(const TCHAR* src, String& dst);

void CUTE_CORE_API ConvertWideToLocal(const WCHAR* src, String& dst);
void CUTE_CORE_API ConvertUTF8ToWide(const String& src, WString& dst);
void CUTE_CORE_API ConvertWideToUTF8(const WCHAR* src, String& dst);

uint64_t CUTE_CORE_API ConvertTimeToEngine(DWORD timeLow, DWORD timeHigh);
void CUTE_CORE_API ConvertTimeToSystem(uint64_t timestamp, DWORD& timeLow, DWORD& timeHigh);

#endif

#ifdef CUTE_OS_FAMILY_WINDOWS
#ifdef CUTE_WIN32_UTF8
#	define TCONVERT(str)			\
	WString w_##str;				\
	ConvertUTF8ToWide(str, w_##str);

#	define TSTR(str)		w_ ## str
#	define LPCTSTR(str)		(LPCWSTR)((w_##str).c_str())
#else
#	define TCONVERT(str)
#	define TSTR(str)		str
#	define LPCTSTR(str)		(LPCSTR)((str).c_str())
#endif
#endif
//////////////////////////////////////////////////////////////////////////
// clock_gettime
//////////////////////////////////////////////////////////////////////////
// check has clock_gettime
#if !defined(CLOCK_REALTIME)
#	define CUTE_EMULATE_CLOCK_GETTIME
#elif !defined(CLOCK_MONOTONIC)
// should be in VxWorks 6.x
#	define CLOCK_MONOTONIC CLOCK_REALTIME
#	define CUTE_NO_CLOCK_MONOTONIC
#endif

#if defined(CUTE_EMULATE_CLOCK_GETTIME)
//////////////////////////////////////////////////////////////////////////
// defined CLOCK MACRO
//////////////////////////////////////////////////////////////////////////
#if defined(__MACH__)
// XXX only supports a single timer
#	define TIMER_ABSTIME				-1
#	define CLOCK_REALTIME				CALENDAR_CLOCK
#	define CLOCK_MONOTONIC				SYSTEM_CLOCK
#else
#	define CLOCK_REALTIME				0 
#	define CLOCK_MONOTONIC				1 
#	define CLOCK_PROCESS_CPUTIME_ID     2	// 本进程到当前代码系统CPU花费的时间
#	define CLOCK_THREAD_CPUTIME_ID      3	// 本线程到当前代码系统CPU花费的时间
#	define CLOCK_REALTIME_HR			4
#	define CLOCK_MONOTONIC_HR			5
#endif
//////////////////////////////////////////////////////////////////////////
// check defined timespec
//////////////////////////////////////////////////////////////////////////
#if defined(CUTE_OS_FAMILY_WINDOWS)
struct timespec
{
	time_t tv_sec; /*second*/
	long tv_nsec;/*nanosecond*/
};
#elif defined(__MACH__)
typedef mach_timespec_t timespec;
#endif

/*!
RETURN VALUE
clock_gettime returns 0 on success; otherwise, it returns one of the errors listed in the "Errors" section.

ERRORS
-EINVAL
An invalid which_clock value was specified.

-EFAULT 
Can not coy to tp value.
*/
long CUTE_CORE_API clock_gettime(int which_clock, struct timespec* tp);

#endif

CUTE_NS_END
