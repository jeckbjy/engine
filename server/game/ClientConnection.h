#include "Cute/TCPConnection.h"
using namespace Cute;

class ClientConnection : public TCPConnection
{
public:
	ClientConnection(socket_t sock);

private:
	User* m_user;
};