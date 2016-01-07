#include "GameClient.h"
#include "Log.h"
#include <iostream>
using namespace std;
//#include "Message.h"

Client gClient;

Client::Client()
{
	m_config.add_host(NET_CONNECTOR, "127.0.0.1:6666");
}

IProtocol* Client::getProtocol(int type)
{
	return TextProtocol::InstancePtr();
}

bool Client::onEvent(NetEvent* ev)
{
	if (ev->isKindOf<TextEvent>())
	{
		TextEvent* tev = (TextEvent*)ev;
		tev->sess->send("client chat\r\n");
		cout << tev->text.toString() << endl;
		//LOG_TRACE("%s", tev->text.toString().c_str());
	}

	return true;
}

void Client::onConnect(Session* sess)
{
	//ChatMsg msg;
	//msg.chat = "client chat";
	//m_connector->sendMsg(&msg);
	cout << "connect,do send" << endl;
	sess->send("client chat \r\n");
}

void Client::onError(Session* sess, error_t ec)
{
	cout<<"Á¬½ÓÊ§°Ü\t"<<ec<<endl;
}