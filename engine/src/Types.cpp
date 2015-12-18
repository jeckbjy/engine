#include "Types.h"

CU_NS_BEGIN

#ifdef CU_OS_WIN
LPFN_CONNECTEX FConnectEx = NULL;
LPFN_ACCEPTEX  FAcceptEx = NULL;
struct NetInit
{
	NetInit()
	{
		// 注册
		WSADATA wsa_data;
		::WSAStartup(MAKEWORD(2, 2), &wsa_data);

		// 获得函数指针
		socket_t sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, 0, 0, WSA_FLAG_OVERLAPPED);
		assert(sock != INVALID_SOCKET);

		DWORD bytes = 0;
		GUID guid_connectex = WSAID_CONNECTEX;
		DWORD res = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guid_connectex, sizeof(guid_connectex),
			&FConnectEx, sizeof(FConnectEx), &bytes, 0, 0);
		assert(res != SOCKET_ERROR);
		if (res == SOCKET_ERROR)
			FConnectEx = NULL;

		GUID guid_acceptex = WSAID_ACCEPTEX;
		res = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guid_acceptex, sizeof(guid_acceptex),
			&FAcceptEx, sizeof(FAcceptEx), &bytes, 0, 0);
		assert(res != SOCKET_ERROR);
		if (res == SOCKET_ERROR)
			FAcceptEx = NULL;

		::closesocket(sock);
	}
	~NetInit()
	{
		::WSACleanup();
	}
};
NetInit gNet;
#endif

CU_NS_END