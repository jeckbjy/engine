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
#include <signal.h>
#include <cctype>

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <queue>

#ifdef CU_CPP11
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#endif

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// 数据类型
//////////////////////////////////////////////////////////////////////////
#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif
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

using std::size_t;
typedef signed int	ssize_t;

typedef int8_t			sint8;
typedef int16_t			sint16;
typedef int32_t			sint32;
typedef int64_t			sint64;
typedef uint8_t			uint8;
typedef uint16_t		uint16;
typedef uint32_t		uint32;
typedef uint64_t		uint64;
typedef float			float32;
typedef double			float64;

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;

typedef std::string			String;
typedef std::wstring		WString;
typedef std::vector<String>	StringList;

#define NPOS ((unsigned int)-1)

template<class T>
class Vector : public std::vector<T>{};
template<class T>
class HashSet : public std::unordered_set<T>{};
template<class U, class V, class _Hasher = std::hash<U>, class _Keyeq = std::equal_to<U>>
class HashMap : public std::unordered_map<U, V, _Hasher, _Keyeq>{};
//template<class T>
//using Vector = std::vector<T>;
//template<class T>
//using HashSet = std::unordered_set<T>;
//template<class U, class V>
//using HashMap = std::unordered_map<U, V>;

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
typedef long	ioctl_req_t;
#else
#   define MAX_PATH				MAXPATHLEN
#	define INVALID_HANDLE_VALUE 0
#	define INVALID_SOCKET		0
typedef int		handle_t;
typedef void*	module_t;
typedef int		socket_t;
typedef int		error_t;
#ifdef CU_OS_BSD
typedef ulong	ioctl_req_t;
#else
typedef int		ioctl_req_t;
#endif
#endif

#ifdef _WIN32
extern LPFN_CONNECTEX		FConnectEx;
extern LPFN_ACCEPTEX		FAcceptEx;
#define cu_close_socket(s)	::closesocket(s)
#define cu_close_handle(h)	::CloseHandle(h)
#define cu_last_error()		::GetLastError()
#define snprintf			sprintf_s
// 忽略字符大小写，strncasecmp比较前n个字符
#define strcasecmp			stricmp
#define strncasecmp			strnicmp 
#else
#define cu_close_socket(s)	::close(s)
#define cu_close_handle(s)	::close(s)
#define cu_last_error()		errno
#endif

// 因为linux会被打断，封装一层
extern int sock_recv(socket_t sock, void* buff, int size, int flags = 0);
extern int sock_send(socket_t sock, const void* buff, int size, int flags = 0);
extern int sock_recvfrom(socket_t sock, void* buff, int size, int flags, struct sockaddr* from, int fromlen);
extern int sock_sendto(socket_t sock, const void* buff, int size, int flags, const struct sockaddr* to, int tolen);

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#ifndef S_IFLNK
# define S_IFLNK 0xA000
#endif

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
	ERR_ALREADY_OPEN			= 1,
	ERR_EOF						= 2,	// 连接正常关闭
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

inline error_t getLastError()
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
