#pragma once
#include "Proto.h"
using namespace cute;
using namespace std;

enum MsgDefine
{
	S2C_LOGIN = 1,
	S2C_CHAT = 2,
	SOC_INFO = 3,
};

struct ChatMsg
{
	enum { MSG_ID = S2C_CHAT,};
};

struct LoginMsg
{
	enum { MSG_ID = S2C_LOGIN, };
};
