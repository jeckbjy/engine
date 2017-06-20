#pragma once
#include "Cute/Platform.h"

#ifdef _WIN32
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
#	include <stdarg.h>
// 多线程
#	include<pthread.h>

#if defined(__MACH__)
#	include <mach/mach.h>
#	include <mach/clock.h>
#	include <mach/mach_time.h>
#endif

#endif

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <stddef.h>
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

#ifdef CUTE_CPP11
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#else
#include <hash_map>
#include <hash_set>
#endif
