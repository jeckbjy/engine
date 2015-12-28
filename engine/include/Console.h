#pragma once
#include "API.h"

CU_NS_BEGIN

// 控制台输出颜色
enum ColorType
{
	CC_BLACK,
	CC_RED,
	CC_GREEN,
	CC_BROWN,
	CC_BLUE,
	CC_MAGENTA,
	CC_CYAN,
	CC_GREY,
	CC_YELLOW,
	//
	CC_LRED,
	CC_LGREEN,
	CC_LBLUE,
	CC_LMAGENTA,
	CC_LCYAN,
	CC_WHITE,
	CC_COLOR_MAX,			// 最大颜色

	CC_DEFAULT = CC_GREY,	// 默认原始颜色
};

class CU_API Console
{
public:
	static void setCloseButton(bool enable);
	static void setColor(ColorType color = CC_DEFAULT);
	static void printf(ColorType color, const char* fmt, ...);
};

CU_NS_END