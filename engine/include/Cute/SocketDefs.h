#pragma once
#include "Cute/Foundation.h"

#if defined(CUTE_OS_FAMILY_WINDOWS)
#	include <winsock2.h>
#	include <ws2tcpip.h>

#elif (CUTE_OS == CUTE_OS_VXWORKS)
#	include <hostLib.h>
#	include <ifLib.h>
#	include <inetLib.h>
#	include <ioLib.h>
#	include <resolvLib.h>
#	include <types.h>
#	include <socket.h>
#	include <netinet/tcp.h>

#elif defined(CUTE_OS_FAMILY_UNIX) || defined(CUTE_OS_FAMILY_VMS)
#	include <unistd.h>
#	include <errno.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/un.h>
#	include <fcntl.h>

#	include <sys/ioctl.h>
#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <netdb.h>

#	if defined(CUTE_OS_FAMILY_UNIX)
#	include <net/if.h>
#	endif

#	if CUTE_OS != CUTE_OS_HPUX
#	include <sys/select.h>
#	endif

#if defined(CUTE_OS_FAMILY_VMS)
#	include <inet.h>
#	else
#	include <arpa/inet.h>
#	endif

#	if (CUTE_OS == CUTE_OS_SOLARIS) || (CUTE_OS == CUTE_OS_MAC_OS_X)
#	include <sys/sockio.h>
#	include <sys/filio.h>
#	endif

#endif

CUTE_NS_BEGIN

#ifdef CUTE_OS_FAMILY_WINDOWS

extern LPFN_CONNECTEX	FUN_CONNECTEX;
extern LPFN_ACCEPTEX	FUN_ACCEPTEX;
struct sockaddr_un
{
	USHORT	sun_family;
	char	sun_path[108];
};

#endif

// 因为linux会被打断，封装一层
socket_t CUTE_CORE_API sock_create(int af /* = AF_INET */, int type /* = SOCK_STREAM */, int proto /* = IPPROTO_IP */);
socket_t CUTE_CORE_API sock_accept(socket_t listener, sockaddr* addr, socklen_t* len);

void CUTE_CORE_API sock_bind(socket_t sock, const struct sockaddr* addr, int len);
void CUTE_CORE_API sock_listen(socket_t sock, int backlog);

int CUTE_CORE_API sock_close(socket_t sock);
int CUTE_CORE_API sock_recv(socket_t sock, void* buff, int size, int flags = 0);
int CUTE_CORE_API sock_send(socket_t sock, const void* buff, int size, int flags = 0);
int CUTE_CORE_API sock_recvfrom(socket_t sock, void* buff, int size, int flags, struct sockaddr* from, int fromlen);
int CUTE_CORE_API sock_sendto(socket_t sock, const void* buff, int size, int flags, const struct sockaddr* to, int tolen);
int CUTE_CORE_API sock_connect(socket_t sock, const struct sockaddr* addr, int len);
int CUTE_CORE_API sock_ioctl(socket_t sock, ioctl_req_t request, void* arg);
int CUTE_CORE_API sock_readable(socket_t sock);

void CUTE_CORE_API sock_setoption(socket_t sock, int level, int option, const void* value, socklen_t length);
void CUTE_CORE_API sock_getoption(socket_t sock, int level, int option, void* value, socklen_t length);
void CUTE_CORE_API sock_setoption(socket_t sock, int level, int option, int value);
void CUTE_CORE_API sock_getoption(socket_t sock, int level, int option, int& value);
int  CUTE_CORE_API sock_getoption(socket_t sock, int level, int option);

void CUTE_CORE_API sock_setreuseport(socket_t sock, bool flag);
bool CUTE_CORE_API sock_getreuseport(socket_t sock);
void CUTE_CORE_API sock_setlinger(socket_t sock, bool on, int seconds);
void CUTE_CORE_API sock_getlinger(socket_t sock, bool& on, int& seconds);
void CUTE_CORE_API sock_setblocking(socket_t sock, bool flag);

class SocketAddress;
void CUTE_CORE_API sock_getlocal(socket_t sock, SocketAddress& addr);
void CUTE_CORE_API sock_getpeer(socket_t sock, SocketAddress& addr);

namespace AddressFamily
{
	enum Family
	{
		IPv4 = AF_INET,
		IPv6 = AF_INET6,
		Unix = AF_UNIX,
	};
};

enum SocketMode
{
	SOCK_MODE_TCP,
	SOCK_MODE_UDP,
	SOCK_MODE_LOCAL,
};

enum SocketOptionMask
{
	MASK_SO_BIND_ANY	= 0x01,
	MASK_SO_BLOCKING	= 0x02,
	MASK_SO_NODELAY		= 0x04,
	MASK_SO_KEEPALIVE	= 0x08,
	MASK_SO_OOBINLINE	= 0x10,
	MASK_SO_BROADCAST	= 0x20,
	MASK_SO_REUSE_ADDR	= 0x40,
	MASK_SO_REUSE_PORT	= 0x80,
	MASK_SO_SNDBUF		= 0x100,
	MASK_SO_RCVBUF		= 0x200,
	MASK_SO_ERROR		= 0x400,
	MASK_SO_TYPE		= 0x800,

	MASK_SO_DEFAULT		= 0,
	MASK_SO_REUSE		= MASK_SO_REUSE_ADDR | MASK_SO_REUSE_PORT,
};

CUTE_NS_END
