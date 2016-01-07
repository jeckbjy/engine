#pragma once
#include "Packet.h"
using namespace cute;
using namespace std;

enum MsgDefine
{
	S2C_LOGIN = 1,
	S2C_CHAT = 2,
	SOC_INFO = 3,
};

struct ChatMsg : TPacket<S2C_CHAT>
{
	void encode(pt_encoder& stream) const
	{

	}

	void decode(pt_decoder& stream)
	{

	}
};

struct LoginMsg : TPacket<S2C_LOGIN>
{
	void encode(pt_encoder& stream) const
	{

	}

	void decode(pt_decoder& stream)
	{

	}
};
