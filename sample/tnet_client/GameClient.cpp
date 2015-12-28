#include "GameClient.h"
#include <iostream>
using namespace std;
//#include "Message.h"

Client gClient;

Client::Client()
{
	m_config.add_host(NetInfo::T_CONNECT, "127.0.0.1:6666");
}

IProtocol* Client::getProtocol(int type)
{
	return TextProtocol::InstancePtr();
}

void Client::onConnect(Session* sess)
{
	//ChatMsg msg;
	//msg.chat = "client chat";
	//m_connector->sendMsg(&msg);
	sess->send("client chat \r\n");
}

void Client::onError(Session* sess)
{
	cout<<"Á¬½ÓÊ§°Ü"<<endl;
}