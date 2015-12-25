#pragma once
#include "Proto.h"
using namespace cute;
using namespace std;

enum MsgDefine
{
	S2C_CHAT = 1,
};

struct ChatMsg
{
	enum { MSG_ID = S2C_CHAT, };
};
