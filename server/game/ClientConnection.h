using namespace Cute;

class ClientConnection
{
public:
	ClientConnection(socket_t sock);

private:
	User* m_user;
};