#include "Client.h"
#include "Message.h"

Client gClient;

void Client::onConnect(TcpSession* sess)
{
	ChatMsg msg;
	msg.chat = "client chat";
	m_connector->sendMsg(&msg);
}

void Client::onError(TcpSession* sess, int code)
{
	cout<<"Á¬½ÓÊ§°Ü"<<endl;
}