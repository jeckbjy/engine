#pragma once
#include "Platform.h"
//////////////////////////////////////////////////////////////////////////
// ��ͬƽ̨����ͷ�ļ�
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
// �����
#	include <sys/select.h>		// ע��HPUXϵͳû�����Ŀ¼
#	include <arpa/inet.h>
#	include <net/if.h>
#	include <stdarg.h>
// ���߳�
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
// ��������
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

template<class T>
using Vector = std::vector<T>;
template<class T>
using Atomic = std::atomic<T>;
typedef Atomic<int8_t>	Atomic8;
typedef Atomic<int16_t>	Atomic16;
typedef Atomic<int32_t> Atomic32;

//////////////////////////////////////////////////////////////////////////
// ��ͬƽ̨�����Ͳ���
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#	define SHUT_RD		SD_RECEIVE
#	define SHUT_WR		SD_SEND
#	define SHUT_RDWR	SD_BOTH

// posix������������
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
extern LPFN_CONNECTEX		FConnectEx;
extern LPFN_ACCEPTEX		FAcceptEx;
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

#ifdef _WIN32
#define CU_ERROR(ec) WSA ## ec
#define CU_WIN_OR_POSIX(e_win, e_posix) e_win
#else
#define CU_ERROR(ec) ec
#define CU_WIN_OR_POSIX(e_win, e_posix) e_posix
#endif

enum ErrorCode
{
	ERR_SUCCESS					= 0,
	ERR_ACCESS					= CU_ERROR(EACCES),
	ERR_AFNOSUPPORT				= CU_ERROR(EAFNOSUPPORT),
	ERR_ADDRESS_IN_USE			= CU_ERROR(EADDRINUSE),
	ERR_ALREADY_CONNECTED		= CU_ERROR(EISCONN),
	ERR_ALREADY_STARTED			= CU_ERROR(EALREADY),
	ERR_CONNECTION_ABORTED		= CU_ERROR(ECONNABORTED),
	ERR_CONNECTION_REFUSED		= CU_ERROR(ECONNREFUSED),
	ERR_CONNECTION_RESET		= CU_ERROR(ECONNRESET),
	ERR_BAD_DESCRIPTOR			= CU_ERROR(EBADF),
	ERR_FAULT					= CU_ERROR(EFAULT),
	ERR_HOST_UNREACHABLE		= CU_ERROR(EHOSTUNREACH),
	ERR_IN_PROGRESS				= CU_ERROR(EINPROGRESS),
	ERR_INTERRUPTED				= CU_ERROR(EINTR),
	ERR_INVALID_ARGUMENT		= CU_ERROR(EINVAL),
	ERR_NAME_TOO_LONG			= CU_ERROR(ENAMETOOLONG),
	ERR_NETWORK_DOWN			= CU_ERROR(ENETDOWN),
	ERR_NETWORK_RESET			= CU_ERROR(ENETRESET),
	ERR_NETWORK_UNREACHABLE		= CU_ERROR(ENETUNREACH),
	ERR_NO_DESCRIPTORS			= CU_ERROR(EMFILE),
	ERR_NO_BUFFER_SPACE			= CU_ERROR(ENOBUFS),
	ERR_NO_PROTOCOL_OPTION		= CU_ERROR(ENOPROTOOPT),
	ERR_NOT_CONNECTED			= CU_ERROR(ENOTCONN),
	ERR_NOT_SOCKET				= CU_ERROR(ENOTSOCK),
	ERR_OPERATION_NOT_SUPPORTED = CU_ERROR(EOPNOTSUPP),
	ERR_SHUT_DOWN				= CU_ERROR(ESHUTDOWN),
	ERR_TIMED_OUT				= CU_ERROR(ETIMEDOUT),
	ERR_WOULD_BLOCK				= CU_ERROR(EWOULDBLOCK),
	ERR_BROKEN_PIPE				= CU_WIN_OR_POSIX(ERROR_BROKEN_PIPE, EPIPE),
	ERR_NO_MEMORY				= CU_WIN_OR_POSIX(ERROR_OUTOFMEMORY, ENOMEM),
	ERR_NO_PERMISSION			= CU_WIN_OR_POSIX(ERROR_ACCESS_DENIED, EPERM),
	ERR_OPERATION_ABORTED		= CU_WIN_OR_POSIX(ERROR_OPERATION_ABORTED, ECANCELED),
	ERR_RETRY_AGAIN				= CU_WIN_OR_POSIX(ERROR_RETRY, EAGAIN),
};

inline error_t last_error()
{
#ifdef _WIN32
	return ::GetLastError();
#else
	return errno;
#endif
}

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