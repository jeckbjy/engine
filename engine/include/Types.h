#pragma once
#include "Platform.h"
//////////////////////////////////////////////////////////////////////////
// 不同平台常用头文件
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#	define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <windows.h>
#	include <WinSock2.h>
#	include <WS2tcpip.h>
#	include <mswsock.h>
#	include <io.h>
#	include <tchar.h>
#else
#	include <stdio.h>
#	include <unistd.h>
#	include <fcntl.h>
#	include <errno.h>
#	include <utime.h>
#	include <signal.h>

#	include <sys/stat.h>
#	include <sys/types.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#   include <sys/param.h>

#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <netdb.h>
// 特殊的
#	include <sys/select.h>		// 注：HPUX系统没有这个目录
#	include <arpa/inet.h>
#	include <net/if.h>
#	include <stdarg.h>
// 多线程
#	include<pthread.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>

#include <atomic>

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// 数据类型
//////////////////////////////////////////////////////////////////////////
#if _UNICODE
typedef wchar_t tchar_t;
#define tvsprintf vswprintf
#else
typedef char tchar_t;
#define tvsprintf vsprintf
#endif

#ifndef _T
#ifdef _UNICODE
#define __T(x) L##x
#else
#define __T(x)	x
#endif
#define _T(x)	__T(x)
#endif

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned int	size_t;
typedef std::basic_string<char>		String;
typedef std::basic_string<wchar_t>  WString;
typedef std::vector<String>			StringList;

//////////////////////////////////////////////////////////////////////////
// 不同平台间类型差异
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#	define SHUT_RD		SD_RECEIVE
#	define SHUT_WR		SD_SEND
#	define SHUT_RDWR	SD_BOTH

// posix有这两个定义
typedef int		socklen_t;
typedef DWORD	pid_t;
// 
typedef HANDLE	handle_t;
typedef HMODULE module_t;
typedef SOCKET	socket_t;
typedef DWORD	error_t;
#else
#   define MAX_PATH MAXPATHLEN
#	define INVALID_HANDLE_VALUE -1
#	define INVALID_SOCKET		-1
typedef int		handle_t;
typedef void*	module_t;
typedef int		socket_t;
typedef int		error_t;
#endif

#ifdef _WIN32
#define strcasecmp			stricmp
#define strncasecmp			strnicmp 
#define cu_close_socket(s)	::closesocket(s)
#define cu_close_handle(h)	::CloseHandle(h)
#define cu_last_error()		::GetLastError()
#else
#define cu_close_socket(s)	::close(s)
#define cu_close_handle(s)	::close(s)
#define cu_last_error()		errno
#endif

template<class T>
using Vector = std::vector<T>;
template<class T>
using Atomic = std::atomic<T>;
typedef Atomic<int8_t>	Atomic8;
typedef Atomic<int16_t>	Atomic16;
typedef Atomic<int32_t> Atomic32;

template<typename T>
struct DeletePolicy
{
	static void destroy(T* ptr) { delete ptr; }
};

template<typename T>
struct ReleasePolicy
{
	static void destroy(T* ptr) { ptr->release(); }
};

CU_NS_END