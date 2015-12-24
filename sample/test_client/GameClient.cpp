#include "GameClient.h"
#include <iostream>
using namespace std;
//#include "Message.h"

Client gClient;

void Client::onConnect(Session* sess)
{
	//ChatMsg msg;
	//msg.chat = "client chat";
	//m_connector->sendMsg(&msg);
}

void Client::onError(Session* sess)
{
	cout<<"Á¬½ÓÊ§°Ü"<<endl;
}