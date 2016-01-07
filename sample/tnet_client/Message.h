#pragma once
#include "Packet.h"
using namespace cute;
using namespace std;

enum MsgDefine
{
	S2C_CHAT = 1,
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
